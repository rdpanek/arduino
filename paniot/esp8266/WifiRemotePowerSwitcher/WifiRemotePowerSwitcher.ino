#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <RCSwitch.h>

int LED = 4;

void blick(int count, int _delay)
{
  for (int _count = 0; _count <= count; _count++) {
    digitalWrite(LED, HIGH);
    delay(_delay);
    digitalWrite(LED, LOW);
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // print the ssid that we should connect to to configure the ESP8266
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());

  blick(6,50);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
    wifiManager.setConfigPortalTimeout(60);
    if(!wifiManager.autoConnect("WifiSwitch")) {
      Serial.println("failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    } 


    Serial.println("Connection succesfull");
    blick(2,100);

}

void loop() {
}
