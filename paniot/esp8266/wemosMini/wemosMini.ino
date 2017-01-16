#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "led.h"
#include "SSD1306.h"
#include "DialogPlain14Bold.h"
#include "DialogPlain12.h"
#include "DialogPlain10.h"
#include <OneWire.h>
#include <DallasTemperature.h>

SSD1306 display(0x3c, D2, D1);

String nazevZarizeni = "Muj-teplotni-senzor";
int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;
char ipAdresa[16];
krok = 0;
const int ldrPin = 0;
const int pinCidlaDS = D3;
float dallasTemperature = 0;

OneWire oneWireDS(pinCidlaDS);
DallasTemperature dallasDS(&oneWireDS);


WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  
  // blikne jednou - ceka se na zadani uzivatele
  ledBlick(1, 300);
  displayMessage(String("Cekam"), String("192.168.4.1"), 200);
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
  
  // blikne dvakrat - nastaveni wifi ulozeno
  ledBlick(2, 300);
  displayMessage(String("Wi-Fi"), String("Ulozeno"), 200);
}

void mainFrame() {
  display.drawRect(displayMarginLeft + 1, displayMarginTop + 3, 64, 48);
}

void displayMessage(String firstLine, String secondLine, int _delay) {
  //mainFrame();
  display.setFont(Dialog_plain_10);
  display.drawString(64, displayMarginTop + 10, firstLine);
  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, secondLine);
  display.display();
  delay(_delay);
  display.clear();
}

int configPortalTimeout = 120;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  display.displayOn();
  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, "Wi-Fi");
  display.display();
  delay(2000);
  display.clear(); 
  
  //wifiManager.resetSettings();
  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  char* _nazevZarizeni = &nazevZarizeni[0];
  wifi_station_set_hostname(_nazevZarizeni);
  
  if(!wifiManager.autoConnect(_nazevZarizeni)) {
    Serial.println("failed to connect and hit timeout");

    ledBlick(3, 300);
    displayMessage(String("Wi-Fi"), String("Reset"), 200);
    
    ESP.reset();
    delay(1000);
  }

  IPAddress ip = WiFi.localIP();
  sprintf(ipAdresa, "%d.%d", ip[2], ip[3]);
  Serial.println("-- start --");
}

void loop() {
  ++krok;
  if ( krok == 1 ) {
    displayMessage("RSSI", String(WiFi.RSSI())+" dBm", 200);
  } else if ( krok == 2 ) {
    displayMessage("IP", String(ipAdresa), 200);
  } else if ( krok == 3 ) {
    displayMessage("LDR", String(analogRead(ldrPin)), 200);
  } else if ( krok == 4 ) {
    displayMessage("Free Heap", String(ESP.getFreeHeap()), 200);
  } else if ( krok == 5 ) {
    dallasDS.requestTemperatures();
    dallasTemperature = dallasDS.getTempCByIndex(0);
    displayMessage("Teplota", String(dallasTemperature)+" °C", 200);
  }

  if ( krok == 5 ) krok = 0;

  delay(4000);
} 

