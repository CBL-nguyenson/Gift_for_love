#define time_delay(t) for (static uint16_t _lasttime; \
                           (uint16_t)((uint16_t)millis() - _lasttime) >= (t); \
                           _lasttime += (t))






//=======virtualPin======
#define Getdata_Temp V1
#define Getdata_Hum V3
#define Putdata_Temp V2
#define Putdata_Hum V4
#define Update_Code V50
//========Variable========
float Var_Getdata_Temp;
float Var_Getdata_Hum;
float Var_Putdata_Temp;
float Var_Putdata_Hum;
float Var_Update_Code;

//=======Pin======

#define buton_touch D6
#define led D2
#define l1 A0
#define mr D8

//==============================================================
//========================= Wifi_ Connect ======================
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

void restartMCU() {
  ESP.restart();
  delay(10000);
  ESP.reset();
  while (1) {};
}

void wifi_reset() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}
void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());                       // in ra địa chỉ wifi
  Serial.println(myWiFiManager->getConfigPortalSSID());  //in ra tên wifi sẽ phát ra
}
//========================= Wifi_OTA ======================

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
ESP8266WiFiMulti WiFiMulti;
void Update_OTA(){
  ESPhttpUpdate.setClientTimeout(2000);  // default was 8000

  WiFi.mode(WIFI_STA);
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, "http://server/file.bin");
    // Or:
    // t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");
    switch (ret) {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
  }
}

//==============================================================
//========================= Device ======================
#include "OneButton.h"
#define PIN_INPUT_PORT D3
OneButton bt_port(PIN_INPUT_PORT, true);

//===========AHT(TEMP)===========
#include <Adafruit_AHTX0.h>
Adafruit_AHTX0 aht;

void Setup_AHT() {
  aht.begin();
}

void loop_AHT() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Var_Putdata_Temp = temp.temperature;
  Var_Putdata_Hum = humidity.relative_humidity;
  Blynk.virtualWrite(Putdata_Temp, temp.temperature);
  Blynk.virtualWrite(Putdata_Hum, humidity.relative_humidity);
}

//===========OLED===========
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeSans9pt7b.h>

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);  //Khai báo màn hình
void Setup_Oled() {
  display.begin(0x3C, true);
  display.clearDisplay();

  display.display();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("WIFI IS CONNECT");
  display.display();
  display.setFont(&FreeSans9pt7b);
  delay(1000);
  display.clearDisplay();
}
void loop_Oled() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("#DTQT");

  display.setCursor(0, 40);
  display.print("Temp: ");
  display.print(Var_Putdata_Temp);
  display.println(" C");

  display.setCursor(0, 60);
  display.print("Hum : ");
  display.print(Var_Putdata_Hum);
  display.println(" %");
  display.display();
}


//===========RUN===========
void Setup_Run() {  // Run in void_Setup
  Serial.begin(9600);
  pinMode(buton_touch, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(l1, INPUT);
  pinMode(mr, OUTPUT);
  bt_port.attachLongPressStart(wifi_reset);
  Setup_AHT();
  Setup_Oled();
}

void Loop_Run() {

  time_delay(5000) {
    loop_AHT();
    loop_Oled();
  }
}
