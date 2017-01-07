// senzors
#include "rtc.h"
#include "ldr.h"
#include "dallas.h"
#include "led.h"
#include "wifiManagerSetup.h"

String devicePlace = "pracovna";
#include "elasticsearch.h"

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  ledBlick(1,10);
  Serial.begin(115200);
  initRTC();
  displayTime();
  dallasDS.begin();
  wifiManagerInit();


  // vse nastaveno, startuje se
  Serial.println("-- start --");
  ledBlick(2,100);
}

void loop() {
  ds18b20ToElastic();
}

