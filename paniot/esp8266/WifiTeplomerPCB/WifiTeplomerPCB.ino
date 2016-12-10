// senzors
#include "rtc.h"
#include "ldr.h"
#include "dallas.h"
#include "led.h"
#include "wifiManagerSetup.h"

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  initRTC();
  displayTime();
  dallasDS.begin();
  //wifiManagerInit();


  // vse nastaveno, startuje se
  Serial.println("-- start --");
  blick(2,100);
}

void loop() {
  //dallasDS.requestTemperatures();
  //Serial.println(dallasDS.getTempCByIndex(0));
  blick(2,1000);
}

