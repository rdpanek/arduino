#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>
#include <string.h>
#include "httpDate.h"
#include "teplotniCidlo.h"

ADC_MODE(ADC_VCC);
int LED = 4;
int delayBetweenMeasurement = 10000;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  String text = String((char *) &payload[0]);
  char * textC = (char *) &payload[0];
  String rssi;

  switch(type) {
      case WStype_DISCONNECTED:
          Serial.println("Disconnected! " + num);
          break;
      case WStype_CONNECTED:
          {
              IPAddress ip = webSocket.remoteIP(num);
              Serial.println("Connected from: " + num + ip[0] + ip[1] + ip[2] + ip[3]);
              delay(5);
              webSocket.sendTXT(num, "C");
          }
          break;
      case WStype_TEXT:
          switch(payload[0]){
            case 'w': case 'W':
              rssi = String(WiFi.RSSI());
              delay(5);
              webSocket.sendTXT(0,rssi);
              break;
            case 'p':
              Serial.println("Got message: " + num);
              delay(5);
              webSocket.sendTXT(0,"pong");
              break;
            case 'e': case 'E':
              delay(5);
              webSocket.sendTXT(0,text);
              break;
            default:
              delay(5);
              webSocket.sendTXT(0,"**** UNDEFINED ****");
              Serial.println("Got UNDEFINED message: " + num);
              break;
          }
          break;
      
      case WStype_BIN:
          Serial.println("get binary lenght:" + num + lenght);
          hexdump(payload, lenght);
          break;
      }
}

void blick(int count, int _delay)
{
  for (int _count = 0; _count < count; _count++) {
    digitalWrite(LED, HIGH);
    delay(_delay);
    digitalWrite(LED, LOW);
    delay(_delay);
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  blick(3,100);
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
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

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println("Inicializuji SPIFFS");
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  
  Serial.println("SPIFFS inicializovan");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {    
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
  }
  Serial.printf("\n");
  
  
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

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {

  server.handleClient();
  webSocket.loop();

  String jsonData;
  jsonData += "{\"umisteni\": \"pracovna\"";
  jsonData += ",\"teplota\": \"";
  char tmp[5]; 
  dtostrf(getTeplota(), 1, 2, tmp);
  jsonData += tmp;
  jsonData += "\"";
  jsonData += ",\"chip\": \""+getDSOChip()+"\"";
  jsonData += ",\"freeHeap\": "+String(ESP.getFreeHeap());
  jsonData += ",\"resetReason\": \""+ESP.getResetReason()+"\"";
  jsonData += ",\"flashChipSpeed\": "+String(ESP.getFlashChipSpeed());
  jsonData += ",\"vcc\":"+String(ESP.getVcc());
  jsonData += ",\"dateTime\":\""+String(getTime())+"\"";
  jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
  jsonData += "}";

  // wait for WiFi connection
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://146.148.102.113:9200/wifi-teplomer/wifi-teplomer/"); //HTTP
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

