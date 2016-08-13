#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <RCSwitch.h>
#include <FS.h>
#define DBG_OUTPUT_PORT Serial
#include <WebSocketsServer.h>
#include <RCSwitch.h>


int LED = 4;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
RCSwitch mySwitch = RCSwitch();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  String text = String((char *) &payload[0]);
  char * textC = (char *) &payload[0];
  String rssi;

  switch(type) {
      case WStype_DISCONNECTED:
          DBG_OUTPUT_PORT.println("Disconnected! " + num);
          break;
      case WStype_CONNECTED:
          {
              IPAddress ip = webSocket.remoteIP(num);
              DBG_OUTPUT_PORT.println("Connected from: " + num + ip[0] + ip[1] + ip[2] + ip[3]);
              delay(5);
              webSocket.sendTXT(num, "C");
          }
          break;
      case WStype_TEXT:
          switch(payload[0]){
            case 'w': case 'W':  // Request RSSI wx
              rssi = String(WiFi.RSSI());
              delay(5);
              webSocket.sendTXT(0,rssi);
              break;
            case 'p': // ping, will reply pong
              DBG_OUTPUT_PORT.println("Got message: " + num);
              delay(5);
              webSocket.sendTXT(0,"pong");
              break;
            case 'e': case 'E':   //Echo
              delay(5);
              webSocket.sendTXT(0,text);
              break;
            case 'B':   // Tlacitko
              delay(5);
              if ( text == "BA1" ) toogleButton(11548893); // Prvni tlacitko zap
              if ( text == "BA2" ) toogleButton(11548892); // Prvni tlacitko zap
              if ( text == "BB1" ) toogleButton(11548895); // Druhe tlacitko zap
              if ( text == "BB2" ) toogleButton(11548894); // Druhe tlacitko zap
              delay(500);
              webSocket.sendTXT(0,text);
              break;
            default:
              delay(5);
              webSocket.sendTXT(0,"**** UNDEFINED ****");
              DBG_OUTPUT_PORT.println("Got UNDEFINED message: " + num);
              break;
          }
          break;
      
      case WStype_BIN:
          DBG_OUTPUT_PORT.println("get binary lenght:" + num + lenght);
          hexdump(payload, lenght);
          break;
}
}

void toogleButton(int code) {
  digitalWrite(LED, 1);
  mySwitch.send(code, 24);
  digitalWrite(LED, 0);
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
  DBG_OUTPUT_PORT.println("Entered config mode");
  DBG_OUTPUT_PORT.println(WiFi.softAPIP());
  DBG_OUTPUT_PORT.print("Created config portal AP ");
  DBG_OUTPUT_PORT.println(myWiFiManager->getConfigPortalSSID());
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

    mySwitch.enableTransmit(5);
    
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
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
    wifiManager.setConfigPortalTimeout(60);
    if(!wifiManager.autoConnect("WifiSwitch")) {
      DBG_OUTPUT_PORT.println("failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    } 


    DBG_OUTPUT_PORT.println("Connection succesfull");
    blick(1,100);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    DBG_OUTPUT_PORT.println("");
    DBG_OUTPUT_PORT.print("IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());
    DBG_OUTPUT_PORT.print("MAC address: ");
    DBG_OUTPUT_PORT.println(WiFi.macAddress());

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
}
