
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Wire.h>
#include <string.h>
#include "SSD1306.h"
#include "DialogPlain14Bold.h"
#include "DialogPlain10.h"
#include "teplotniCidlo.h";
#include <FS.h>
#define DBG_OUTPUT_PORT Serial
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

extern "C" {
#include "user_interface.h"
}
SSD1306 display(0x3c, D2, D1);
int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;
String teplota = "00.00";
String dialogMessage;
IPAddress localIP;
String macAddress;
String rssi;
String nazevZarizeni = "";
const int ldrPin = A0;
int ldr;
const int ledPin = D8;
String elasticsearchUri = "";
int displaySetup = 1;
int displayRotation = 1;
bool ledIndication = true;

// measurement
long delayBetweenMeasurement = 10000;
long measurementJob = 0;
long delayBetweenDisplayRotation = 5000;
long displayRotationJob = 0;

// OTA
#define HOSTNAME "PANIOT-"

void blick(int count, int _delay)
{
  if (ledIndication) {
    for (int _count = 0; _count < count; _count++) {
      digitalWrite(ledPin, HIGH);
      delay(_delay);
      digitalWrite(ledPin, LOW);
      delay(_delay);
    }
  }
}

String jsonData;
#include "sendToElastic.h";

ESP8266WebServer server(80);

void configModeCallback (WiFiManager *myWiFiManager) {
  DBG_OUTPUT_PORT.println("Entered config mode");
  DBG_OUTPUT_PORT.println(WiFi.softAPIP());
  DBG_OUTPUT_PORT.print("Created config portal AP ");
  DBG_OUTPUT_PORT.println(myWiFiManager->getConfigPortalSSID());
  displayMessage("192.168.4.1", "cekam", 100);
  blick(3,30);
}


//format bytes
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

bool handleFileRead(String path){
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
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

void saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["delayBetweenMeasurement"] = delayBetweenMeasurement;
  json["nazevZarizeni"] = nazevZarizeni.c_str();
  json["elasticsearchUri"] = elasticsearchUri.c_str();
  json["displaySetup"] = displaySetup;
  json["displayRotation"] = displayRotation;
  json["ledIndication"] = ledIndication;
  
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    DBG_OUTPUT_PORT.println("Failed to open config file for writing");
    displayMessage("Ukladam", "chyba", 300);
    blick(3,30);
  }

  json.printTo(configFile);
  displayMessage("Ukladam", "OK", 300);
  blick(1,100);
}

void loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    DBG_OUTPUT_PORT.println("Failed to open config file");
    displayMessage("Nastaveni", "chyba", 300);
    blick(3,30);
  }

  size_t size = configFile.size();
  if (size > 1024) {
    DBG_OUTPUT_PORT.println("Config file size is too large");
    displayMessage("Nastaveni", "chyba", 300);
    blick(3,30);
  }
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    DBG_OUTPUT_PORT.println("Failed to parse config file");
    displayMessage("Nastaveni", "chyba", 300);
    blick(3,30);
  }
  
  nazevZarizeni = json["nazevZarizeni"];
  DBG_OUTPUT_PORT.println(nazevZarizeni);
  elasticsearchUri = json["elasticsearchUri"];
  DBG_OUTPUT_PORT.println(elasticsearchUri);
  delayBetweenMeasurement = json["delayBetweenMeasurement"];
  DBG_OUTPUT_PORT.println(delayBetweenMeasurement);
  displayRotation = json["displayRotation"];
  DBG_OUTPUT_PORT.println(displayRotation);
  ledIndication = json["ledIndication"];
  DBG_OUTPUT_PORT.println(ledIndication);
  displaySetup = json["displaySetup"];
  DBG_OUTPUT_PORT.println(displaySetup);
  
  displayMessage("Nastaveni", "OK", 300);
  blick(1,100);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.println("~~ PanIoT Wifi start ~~");
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  display.displayOn();
  digitalWrite(ledPin, LOW);

  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, "PanIoT");
  display.display();
  delay(2000);
  display.clear();

  // Set Hostname.
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    DBG_OUTPUT_PORT.printf("\n");
  }

  //saveConfig();
  loadConfig();

  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(60);
  if(!wifiManager.autoConnect("PanIoT Wifi zarizeni")) {
    DBG_OUTPUT_PORT.println("failed to connect and hit timeout");
    displayMessage("Wifi AP", "chyba", 200);
    blick(3,30);
    ESP.reset();
    delay(1000);
  }
  displayMessage("Pripojuji", "", 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }
  displayMessage("Pripojuji", "OK", 300);
  blick(1,100);

  DBG_OUTPUT_PORT.println(WiFi.localIP());
  localIP = WiFi.localIP();
  macAddress = String(WiFi.macAddress());
  rssi = String(WiFi.RSSI());
  
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  server.on ( "/getTemperature", []() {
    server.send ( 200, "text/plain", "<span id=\"temperature\">"+teplota+" °C</span>" );
    delay(300);
  });
  server.on ( "/getLdr", []() {
    server.send ( 200, "text/plain", "<span id=\"ldr\">"+String(ldr)+"</span>" );
    delay(300);
  });
  server.on ( "/saveSetings", []() {
    delayBetweenMeasurement = server.arg("delayBetweenMeasurement").toInt();
    elasticsearchUri = server.arg("elasticsearchUri");
    nazevZarizeni = server.arg("nazevZarizeni");
    saveConfig();
    server.send ( 200, "text/plain", "OK" );
    delay(300);
  });
  server.on ( "/saveDisplaySetings", []() {
    displaySetup = server.arg("displaySetup").toInt();
    displayRotation = server.arg("displayRotation").toInt();
    saveConfig();
    server.send ( 200, "text/plain", "OK" );
    delay(300);
  });
  server.on ( "/restart", []() {
    server.send ( 200, "text/plain", "Zarizeni bude restartovano." );
    displayMessage("Restart", "OK", 300);
    blick(1,100);
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/factoryrestart", []() {
    server.send ( 200, "text/plain", "Bude nastaveno tovarni nastaveni. Vyhledejte wifi pojmenovanou PanIoT Wifi zarizeni" );
    blick(1,100);
    WiFi.disconnect(true);
    delay(1000);
    displayMessage("Mazani", "OK", 300);
    ESP.reset();
    delay(1000);
  } );
  server.begin();
  displayMessage("Server", "OK", 300);
  blick(1,100);

  // Start OTA server.
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.onStart([]() {
    displayMessage("Aktualiza.", "", 100); blick(1,10);
    DBG_OUTPUT_PORT.println("Start updating ");
  });
  ArduinoOTA.onEnd([]() {
    displayMessage("Aktualiza.", "100 %", 300); blick(1,10);
    DBG_OUTPUT_PORT.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    displayMessage("Aktualiza.", String((progress / (total / 100)))+" %", 0);
    DBG_OUTPUT_PORT.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DBG_OUTPUT_PORT.printf("Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR) displayMessage("Auth", "chyba", 100);
    else if (error == OTA_BEGIN_ERROR) displayMessage("Begin", "chyba", 100);
    else if (error == OTA_CONNECT_ERROR) displayMessage("Connect", "chyba", 100);
    else if (error == OTA_RECEIVE_ERROR) displayMessage("Receive", "chyba", 100);
    else if (error == OTA_END_ERROR) displayMessage("End", "chyba", 100); 
    blick(3,300);
  });
  ArduinoOTA.begin();
  displayMessage("OTA", "OK", 300);
  blick(1,100);
}


void displayMessage(String firstLine, String secondLine, int _delay) {
  mainFrame();
  display.setFont(Dialog_plain_10);
  display.drawString(64, displayMarginTop + 10, firstLine);
  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, secondLine);
  display.display();
  delay(_delay);
  display.clear();
}

void mainFrame() {
  display.drawRect(displayMarginLeft + 1, displayMarginTop + 3, 64, 48);
}

void measureTemperature() {
  teplota = String(getTeplota());
  float _teplota = getTeplota();
  if ( _teplota > 0 ) teplota = _teplota;
}

void measureLDR() {
  ldr = analogRead(ldrPin);
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  
  if (displaySetup == 1) {
    displayMessage("Teplota", teplota, 100);
  } else if (displaySetup == 2) {
    displayMessage("LDR", String(ldr), 100);
  } else if (displaySetup == 3) {
    displayMessage("RSSI", String(rssi), 100);
  } else if (displaySetup == 4) {
    displayMessage("HEAP", String(ESP.getFreeHeap()), 100);
  }

  if (millis() > (displayRotationJob + delayBetweenDisplayRotation)) { 
    if (displayRotation == 1) {
      ++displaySetup;
      if (displaySetup > 4) { displaySetup = 1; }
    }
    displayRotationJob  = millis();
  }

  // measure temperature and ldr
  if (millis() > (measurementJob + delayBetweenMeasurement)) { 
    blick(1,10);
    measureTemperature();
    measureLDR();
    sendTemperatureToElasticsearch();
    sendLdrToElasticsearch();
    measurementJob  = millis();
  }
}

