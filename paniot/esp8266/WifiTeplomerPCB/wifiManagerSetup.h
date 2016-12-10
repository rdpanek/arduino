// esp8266 wifi
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  blick(3,100);
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
  blick(2,100);
}

String deviceName = "PanIoT wifi-temp";
int configPortalTimeout = 120;

void wifiManagerInit() {
  //wifiManager.resetSettings();
  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  if(!wifiManager.autoConnect("PanIoT wifi-temp")) {
    Serial.println("failed to connect and hit timeout");
    blick(3,100);
    ESP.reset();
    delay(1000);
  }
}
