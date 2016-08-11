#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <RCSwitch.h>

int LED = 4;

ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(LED, 0);
}

void handleNotFound(){
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
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

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();


}

void loop() {
  server.handleClient();
}
