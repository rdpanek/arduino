#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <RCSwitch.h>
#include <FS.h>
#define DBG_OUTPUT_PORT Serial

int LED = 4;

ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(LED, 0);
}

void blick(int count, int _delay)
{
  for (int _count = 0; _count <= count; _count++) {
    digitalWrite(LED, HIGH);
    delay(_delay);
    digitalWrite(LED, LOW);
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  DBG_OUTPUT_PORT.println("Entered config mode");
  DBG_OUTPUT_PORT.println(WiFi.softAPIP());
  DBG_OUTPUT_PORT.print("Created config portal AP ");
  DBG_OUTPUT_PORT.println(myWiFiManager->getConfigPortalSSID());

  blick(6,50);
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

void setup() {
    DBG_OUTPUT_PORT.begin(115200);
    DBG_OUTPUT_PORT.print("\n");
    DBG_OUTPUT_PORT.setDebugOutput(true);
    
    pinMode(LED, OUTPUT);

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

    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
    wifiManager.setConfigPortalTimeout(60);
    if(!wifiManager.autoConnect("WifiSwitch")) {
      DBG_OUTPUT_PORT.println("failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    } 


    DBG_OUTPUT_PORT.println("Connection succesfull");
    blick(2,100);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    DBG_OUTPUT_PORT.println("");
    DBG_OUTPUT_PORT.print("IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());

    server.onNotFound([](){
      if(!handleFileRead(server.uri()))
        server.send(404, "text/plain", "FileNotFound");
    });
    server.begin();


}

void loop() {
  server.handleClient();
}
