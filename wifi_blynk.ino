#define BLYNK_TEMPLATE_ID "TMPL6cxAO9ZkA"
#define BLYNK_TEMPLATE_NAME "IOT"
#include "SHT3x.h"
#include <DNSServer.h>
#include <WiFiManager.h>  
#include <BlynkSimpleEsp32.h>
#include <TFT_eSPI.h>//phai de sau include wifi
#define GAS 5
#define BUTTON 17
WiFiManager wm;
TFT_eSPI screen = TFT_eSPI();
SHT3x thsensor;
BlynkTimer timer;
char token[] = "eQJdO-l7Caxtk8-u1NX60NAwj991qa9-";
bool wifiConnect = false;
float temp, humid, gas;
volatile bool wifiConfig = false;
void sensorsRead(){
   thsensor.UpdateData();
  temp = thsensor.GetTemperature();
  humid = thsensor.GetRelHumidity();
  gas = analogRead(GAS);
  gas = map(gas, 0, 1023, 0, 100);
  Serial.println(temp);
  Serial.println(gas);
  Serial.println(humid);
  if(wifiConnect) {
    blynkUpdate();
  }
  screenUpdate();
}

// SCREEN
void screenSetup()
{ screen.setCursor(45,0,2);
  screen.setTextSize(1);
  screen.print("Sensor Hub");
  screen.drawLine(0, 15, 160, 15, TFT_WHITE);
  screen.fillRoundRect(3,20,75,50,10,TFT_BLUE);
  screen.fillRoundRect(82,20,75,50,10,TFT_CYAN);
  screen.fillRoundRect(3,75,75,50,10,TFT_GREEN);
  screen.fillRoundRect(82,75,75,50,10,TFT_YELLOW);
  screen.setTextColor(TFT_BLACK);
  screen.setCursor(5,40,1);
  screen.print("TEMP");
  screen.setCursor(84,40,1);
  screen.print("HUMID");
  screen.setCursor(5,95,1);
  screen.print("GAS");
  screen.setCursor(84,95,1);
  screen.print("WIFI");

}

void screenUpdate()
{
  screen.fillRoundRect(38,20,40,50,10,TFT_BLUE);
  screen.fillRoundRect(117,20,40,50,10,TFT_CYAN);
  screen.fillRoundRect(38,75,40,50,10,TFT_GREEN);
  screen.fillRoundRect(117,75,40,50,10,TFT_YELLOW);
  screen.setCursor(38,40,1);
  screen.print(String(temp,1)+ "÷C");
  screen.setCursor(119,40,1);
  screen.print(String(humid,1)+ "%");
  screen.setCursor(38,95,1);
  screen.print(String(gas,1) + "%");
  screen.setCursor(119,95,1);
  screen.print("V");
  
}

//WIFI
void configWifi() {
  wifiConnect = false;
  wifiConfig = true;

  Serial.println("wifi reconfig");

}
bool wifiSetup()
{
  wm.autoConnect("AutoConnectAP","12345678"); // password protected ap
  WiFiManagerParameter custom_blynk_token("Blynk", "blynk token", token, 34);
  wm.addParameter(&custom_blynk_token); 
  Blynk.config(custom_blynk_token.getValue());
  return Blynk.connect();
}
void blynkUpdate() 
{
  Blynk.virtualWrite(V1, gas);
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, humid);
}


void setup() {
  Serial.begin(115200);
  thsensor.Begin();
  pinMode(GAS, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(26,OUTPUT);
  digitalWrite(26,HIGH);
  screen.init();
  screen.setRotation(1);
  screen.fillScreen(TFT_BLACK);
  screenSetup();
  if(wifiSetup()) {
    Serial.println("BLYNK connected");
    wifiConnect = true;
  } else {
    Serial.println("BLYNK not connected");
  }
  timer.setInterval(2000l,sensorsRead );
  attachInterrupt(digitalPinToInterrupt(BUTTON), configWifi, FALLING);
}


void loop() {
  Blynk.run();
  timer.run();
  if(wifiConfig) {
    wm.resetSettings();
    wm.autoConnect("AutoConnectAP","12345678"); // password protected ap
      if(WiFi.isConnected()) {
    wifiConnect = true;
    }
    wifiConfig = false;
  }

  

}
