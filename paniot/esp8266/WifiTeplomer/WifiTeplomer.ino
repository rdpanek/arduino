#include <ArduinoJson.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <string.h>
#include "httpDate.h"
#include "teplotniCidlo.h"

ADC_MODE(ADC_VCC);
int LED = 4;
long delayBetweenMeasurement = 10000;
long measurementJob = 0;
String elasticUrl;
String nazevTeplomeru;
String lastMeasurementCelsius = "0.0";

ESP8266WebServer server(80);

void blick(int count, int _delay)
{
  for (int _count = 0; _count < count; _count++) {
    digitalWrite(LED, HIGH);
    delay(_delay);
    digitalWrite(LED, LOW);
    delay(_delay);
  }
}

String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void configModeCallback (WiFiManager *myWiFiManager) {
  blick(1,300);
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  elasticUrl = json["elasticUrl"];
  Serial.println(elasticUrl);
  nazevTeplomeru = json["nazevTeplomeru"];
  Serial.println(nazevTeplomeru);
  delayBetweenMeasurement = json["delayBetweenMeasurement"];
  Serial.println(delayBetweenMeasurement);
  return true;
}

bool saveConfig() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["delayBetweenMeasurement"] = delayBetweenMeasurement; //3000;
  json["nazevTeplomeru"] = nazevTeplomeru; //"teplomer";
  json["elasticUrl"] = "http://146.148.102.113:9200/wifi-teplomer/wifi-teplomer/";
  
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  /*
  if (!saveConfig()) {
    Serial.println("Failed to save config");
  } else {
    Serial.println("Config saved");
  }
  */

  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }
  
  
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(60);
  if(!wifiManager.autoConnect("WifiTeplomer")) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  } 

  Serial.println("Connection succesfull");
  blick(3,100);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  server.on ( "/getTemperature", []() {
    server.send ( 200, "text/plain", "<span id=\"temperature\">"+lastMeasurementCelsius+" °C</span>" );
    delay(300);
  } );
  server.on ( "/saveSetings", []() {
    delayBetweenMeasurement = server.arg("measurementInterval").toInt();
    elasticUrl = server.arg("elasticUrl");
    nazevTeplomeru = server.arg("nameThermometer");
    saveConfig();
    server.send ( 200, "text/plain", "OK" );
    delay(300);
  } );
  server.on ( "/restart", []() {
    server.send ( 200, "text/plain", "Zarizeni bude restartovano." );
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/factoryrestart", []() {
    server.send ( 200, "text/plain", "Bude nastaveno tovarni nastaveni. Vyhledejte wifi pojmenovanou WifiSwitch" );
    WiFi.disconnect(true);
    delay(1000);
    ESP.reset();
    delay(1000);
  } );
  server.begin();
}


void measureTemperature() {

  if (nazevTeplomeru.length() == 0) { 
    nazevTeplomeru = "Nazev teplomeru";
  }
  if (elasticUrl.length() == 0) {
    blick(3,100);
    return;
  }

  char temperature[5]; 
  dtostrf(getTeplota(), 1, 2, temperature);
  lastMeasurementCelsius = String(temperature);

  if (getDSOChip().length() < 10) {
    String jsonData;
    jsonData += "{\"umisteni\": \""+String(nazevTeplomeru)+"\"";
    jsonData += ",\"teplota\": \"";
    jsonData += temperature;
    jsonData += "\"";
    jsonData += ",\"chip\": \""+getDSOChip()+"\"";
    jsonData += ",\"freeHeap\": "+String(ESP.getFreeHeap());
    jsonData += ",\"resetReason\": \""+ESP.getResetReason()+"\"";
    jsonData += ",\"flashChipSpeed\": "+String(ESP.getFlashChipSpeed());
    jsonData += ",\"vcc\":"+String(ESP.getVcc());
    jsonData += ",\"dateTime\":\""+String(getTime())+"\"";
    jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
    jsonData += "}";
  
    Serial.println(jsonData);
    Serial.println(elasticUrl);
    Serial.print("DelayBetweenMeasurement: ");
    Serial.println(delayBetweenMeasurement);
    // wait for WiFi connection
    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(elasticUrl);
      http.addHeader("Content-Type","application/json; charset=UTF-8");
      int httpCode = http.POST(jsonData);
      if (httpCode < 300) {
        blick(1,100);
      } else { 
        blick(2,100);
      }
      Serial.println(http.getString());
      http.end();
    }
  }
}

void loop() {
  server.handleClient();

  if (millis() > (measurementJob + delayBetweenMeasurement)) { 
    measureTemperature();
    measurementJob = millis();
  }
}

