#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
String nazevZarizeni = "Muj-teplotni-senzor";

WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

int configPortalTimeout = 120;


void setup() {
  Serial.begin(115200);

  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  char* _nazevZarizeni = &nazevZarizeni[0];
  wifi_station_set_hostname(_nazevZarizeni);
  if(!wifiManager.autoConnect(_nazevZarizeni)) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  server.onNotFound([](){
     server.send(404, "text/plain", "FileNotFound");
  });
    server.on ( "/restart", []() {
    server.send ( 200, "text/plain", "Zarizeni bude restartovano." );
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/factoryRestart", []() {
    server.send ( 200, "text/plain", "Bude nastaveno tovarni nastaveni. Vyhledejte wifi pojmenovanou " + nazevZarizeni );
    WiFi.disconnect(true);
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/", []() {
    String jsonData = "";
    jsonData += "{\"/\": \"Zkusebni endpoint\"";
    jsonData += ",\"/restart\": \"Resetuje zarizeni\"";
    jsonData += ",\"/factoryRestart\": \"Vymaze nastaveni wifi\"";
    jsonData += "}";
    server.send(200, "application/json", jsonData);
  } );
  
  server.begin();

  Serial.println("-- start --");
}

void loop() {
  server.handleClient();
} 

