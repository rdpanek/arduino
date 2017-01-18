String deviceName = "PanIoT-temperature";
String deviceLocation = "kuchyn";

#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
ESP8266WebServer server(80);

// Blynk
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
char auth[] = "";
int blynkDellayMs = 1000; 
int blynkLastOnMS = 0;

// senzors
#include "utility.h"
#include "oled.h"
#include "beep.h"
#include "led.h"
#include "wifiManagerSetup.h"
#include "ntp.h"
#include "elasticsearch.h"
#include "dht22.h"
#include "dallas.h"
#include "ldr.h"

// always on the end
#include "config.h"
#include "ota.h"
#include "webServer.h"


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
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

  oledInit();
  Blynk.config(auth);

  // vse nastaveno, startuje se
  Serial.println("-- start --");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  syncNtp();
  Blynk.run();
  
  measureDallas();
  measureLDR();
  displayMessage(String(WiFi.RSSI())+" dBm", String((int)dallasTemperature)+" °C", 100);

  if (millis() > (blynkDellayMs + blynkLastOnMS)) {
    blynkLastOnMS =  millis();
    Blynk.virtualWrite(1, WiFi.RSSI());
    Blynk.virtualWrite(2, ESP.getFreeHeap());
    Serial.println("send to blynk");
  }
}

