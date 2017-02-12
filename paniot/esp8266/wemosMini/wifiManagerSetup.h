// esp8266 wifi
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266HTTPClient.h>

extern "C" {
#include "user_interface.h"
}

WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ledBlick(1,10,"green");
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
  ledBlick(2,10,"green");
}

int configPortalTimeout = 120;

void wifiManagerInit() {
  //wifiManager.resetSettings();
  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  deviceName += "-" + deviceLocation + "-" + String(ESP.getChipId(), HEX);
  char* _deviceName = &deviceName[0];
  wifi_station_set_hostname(_deviceName);
  if(!wifiManager.autoConnect(_deviceName)) {
    Serial.println("failed to connect and hit timeout");
    ledBlick(1,100,"red");
    ESP.reset();
    delay(1000);
  }
}
