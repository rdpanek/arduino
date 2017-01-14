String deviceName = "PanIoT-webTester";
String deviceLocation = "tovarni";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// senzors
#include "utility.h"
#include "oled.h"
#include "beep.h"
#include "multiLed.h"
#include "wifiManagerSetup.h"
#include "ntp.h"
#include "elasticsearch.h"
#include "webTester.h"

// always on the end
#include "config.h"
#include "ota.h"
#include "webServer.h"


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(multiLed1, OUTPUT);
  ledBlick(1,10,multiLed1);
  beep(1,5);
  pinMode(multiLed2, OUTPUT);
  pinMode(multiLed3, OUTPUT);
  pinMode(multiLed4, OUTPUT);
  pinMode(beepPin, OUTPUT);
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


  ledBlick(1,10,multiLed1);
  beep(1,5);
  delay(50);
  ledBlick(1,10,multiLed2);
  beep(1,5);
  delay(50);
  ledBlick(1,10,multiLed3);
  beep(1,5);
  delay(50);
  ledBlick(1,10,multiLed4);
  beep(1,5);

  // vse nastaveno, startuje se
  Serial.println("-- start --");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  syncNtp();

  // google.cz http://www.google.cz/
  // local turis http.begin("http://192.168.1.1/");
  // roven nas http://79.98.159.232:88/
  // kamera za barakem http.begin("http://79.98.159.232:86/");

  existUri("http://192.168.1.1/", 200, "<title>Administrační rozhraní routeru Turris</title>", multiLed1);
  existUri("http://79.98.159.232:85/", 200, "<TITLE>Archer C7</TITLE>", multiLed2);
  existUri("http://79.98.159.232:84/", 401, "<title>401 - Unauthorized</title>", multiLed3);
  existUri("http://79.98.159.232:86/", 401, "<title>401 - Unauthorized</title>", multiLed4);

  Serial.println(ESP.getFreeHeap());
}

