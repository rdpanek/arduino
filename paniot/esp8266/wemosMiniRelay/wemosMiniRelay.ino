String deviceName = "PanIoT-wifi-rellay";
String deviceLocation = "pracovna";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "wifiManagerSetup.h"
#include "webServer.h"
#include "rellay.h"

String devicePlace = "pracovna";

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(rellayPin, OUTPUT);
  Serial.begin(115200);
  wifiManagerInit();
  webServerInit();
  
  // vse nastaveno, startuje se
  Serial.println("-- start --");
  rellay(true);
  delay(2000);
  rellay(false);
}

void loop() {
  server.handleClient();

  if (apiRelayState) rellay(true);
  if (!apiRelayState) rellay(false);
}

