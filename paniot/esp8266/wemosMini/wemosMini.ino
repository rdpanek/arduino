String deviceName = "PanIoT-wifi-rellay";
String deviceLocation = "pracovna";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "beep.h"
#include "led.h"
#include "wifiManagerSetup.h"
#include "ota.h"
#include "pir.h"
#include "rellay.h"
#include "webServer.h"

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(rellayPin, OUTPUT);
  Serial.begin(115200);
  wifiManagerInit();
  webServerInit();
  initOTA();
  
  // vse nastaveno, startuje se
  Serial.println("-- start --");
  stateRellay = true;
  handleRellay();
  delay(2000);
  stateRellay = false;
  handleRellay();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleRellay();
}

