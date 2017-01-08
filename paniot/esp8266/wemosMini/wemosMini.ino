String deviceName = "PanIoT-wifi-pir";
String deviceLocation = "pracovna";
String rellayUri = "http://192.168.1.197";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "led.h"
#include "beep.h"
#include "wifiManagerSetup.h"
#include "ota.h"
#include "pir.h"
#include "rellay.h"
#include "deviceRequest.h"
#include "webServer.h"

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(beepPin, OUTPUT);
  ledBlick(1,10);
  beep(1,10);
  Serial.begin(115200);
  wifiManagerInit();
  webServerInit();
  initOTA();
  
  // vse nastaveno, startuje se
  Serial.println("-- start --");
  beep(1,10);
  ledBlick(2,100);
  delay(100);
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  isPirActive();
  if (pirState) {
    sendGetRequest(onEndpoint);
  } else {
    sendGetRequest(offEndpoint);
  }
  delay(2000);
}

