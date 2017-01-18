#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
String nazevZarizeni = "Muj-teplotni-senzor";

WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

int configPortalTimeout = 120;
String uri = "http://private-e5a9-tr1.apiary-mock.com/api/v2/results/list/name/date";


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

  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Posilam pozadavek");
    HTTPClient http;
    http.begin(uri);
    http.addHeader("Content-Type","application/json; charset=UTF-8");
    int httpCode = http.GET();
    Serial.println(http.getString());
    if (httpCode > 300) {
      Serial.println("HTTP Code: "+httpCode);
    } else {
      Serial.println(http.getString());  
    }
    http.end();
  } else {
    Serial.println("Wifi neni pripojeno");
  }

  delay(2000);
} 

