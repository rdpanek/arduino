String deviceName = "PanIoT-pir";
String deviceLocation = "pracovna";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "utility.h"
#include "RGB.h"
#include "wifiManagerSetup.h"
#include "ntp.h"
#include "elasticsearch.h"
#include "deviceRequest.h"
#include "rellay.h"
#include "pir.h"

// always on the end
#include "config.h"
#include "ota.h"
#include "webServer.h"


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  initRGB();
  wifiManagerInit();
  webServerInit();
  fsInit();
  initOTA();
  loadConfig();


  // ntp
  if(WiFi.status() == WL_CONNECTED) {
    NTP.begin("pool.ntp.org", 1, true);
    NTP.setInterval(63);
  }

  NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
  });  


  // vse nastaveno, startuje se
  Serial.println("-- start --");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  syncNtp();
  isPirActive();
}

