#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "led.h" // zpristupni knihovnu led


String nazevZarizeni = "Muj-teplotni-senzor";


WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  
  // blikne jednou - ceka se na zadani uzivatele
  ledBlick(1, 300);
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
  
  // blikne dvakrat - nastaveni wifi ulozeno
  ledBlick(2, 300);
}

int configPortalTimeout = 120;

void setup() {
  Serial.begin(115200);

  // definovani pinu kam je pripojena LED dioda
  pinMode(ledPin, OUTPUT);
  
  wifiManager.resetSettings();
  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  char* _nazevZarizeni = &nazevZarizeni[0];
  wifi_station_set_hostname(_nazevZarizeni);
  
  if(!wifiManager.autoConnect(_nazevZarizeni)) {
    Serial.println("failed to connect and hit timeout");

    // blikne trikrat - vyprsel casovy limit
    ledBlick(3, 300);
    
    ESP.reset();
    delay(1000);
  }

  Serial.println("-- start --");
}

void loop() {

  // proces konfigurace wifi je hotovy, ted bude dioda blikat dokola
  ledBlick(1, 50);

} 

