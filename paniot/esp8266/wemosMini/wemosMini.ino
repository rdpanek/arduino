String deviceName = "PanIoT-wifi-dht22";
String deviceLocation = "loznice";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "beep.h"
#include "led.h"
#include "wifiManagerSetup.h"
#include "ota.h"
#include "pir.h"
#include "dht22.h"
#include "webServer.h"

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  wifiManagerInit();
  webServerInit();
  initOTA();

  dht.begin();
  // vse nastaveno, startuje se
  Serial.println("-- start --");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  measureDht22();
}

