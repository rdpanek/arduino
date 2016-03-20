//We always have to include the library
#include "LedControl.h"

/*
 UNO
 pin 12 ( MISO ) is connected to the DataIn  
 pin 11 ( MOSI ) is connected to the CLK 
 pin 10 is connected to LOAD 

 ATTINY
 pin 1 (HW 6) ( MISO ) is connected to the DataIn 
 pin 0 (HW 5) ( MOSI ) is connected to the CLK 
 pin 4 (HW 3) is connected to LOAD 
 */
LedControl lc=LedControl(1,0,4,1);

/* we always wait a bit between updates of the display */
long xRandom;
long yRandom;


void obrisSrdce(int rychlost, boolean zap = true) {
  lc.setLed(0,7,3,zap);
  lc.setLed(0,7,4,zap);
  delay(rychlost);
  lc.setLed(0,6,2,zap);
  lc.setLed(0,6,5,zap);
  delay(rychlost);
  lc.setLed(0,5,1,zap);
  lc.setLed(0,5,6,zap);
  delay(rychlost);
  lc.setLed(0,4,0,zap);
  lc.setLed(0,4,7,zap);
  delay(rychlost);
  lc.setLed(0,3,0,zap);
  lc.setLed(0,3,7,zap);
  delay(rychlost);
  lc.setLed(0,2,0,zap);
  lc.setLed(0,2,7,zap);
  delay(rychlost);
  lc.setLed(0,1,1,zap);
  lc.setLed(0,1,6,zap);
  delay(rychlost);
  lc.setLed(0,1,2,zap);
  lc.setLed(0,1,5,zap);
  delay(rychlost);
  lc.setLed(0,2,3,zap);
  lc.setLed(0,2,4,zap);
  delay(rychlost);
}

void showRandom() {
  int _count = 0;
  while(_count < 400){
    lc.setLed(0,random(8),random(8),true);
    delay(1);
    _count++;
  }  
}

void vyplnSrdce(int rychlost, boolean zap = true) {
  lc.setLed(0,6,3,zap);
  lc.setLed(0,6,4,zap);
  delay(rychlost);
  lc.setLed(0,5,3,zap);
  lc.setLed(0,5,4,zap);
  lc.setLed(0,5,2,zap);
  lc.setLed(0,5,5,zap);
  delay(rychlost);
  lc.setLed(0,4,3,zap);
  lc.setLed(0,4,4,zap);
  lc.setLed(0,4,2,zap);
  lc.setLed(0,4,5,zap);
  lc.setLed(0,4,1,zap);
  lc.setLed(0,4,6,zap);
  delay(rychlost);
  lc.setLed(0,3,3,zap);
  lc.setLed(0,3,4,zap);
  lc.setLed(0,3,2,zap);
  lc.setLed(0,3,5,zap);
  lc.setLed(0,3,1,zap);
  lc.setLed(0,3,6,zap);
  delay(rychlost);
  lc.setLed(0,2,1,zap);
  lc.setLed(0,2,2,zap);
  lc.setLed(0,2,6,zap);
  lc.setLed(0,2,5,zap);
  delay(rychlost);
  
}

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  showRandom();
  obrisSrdce(10, false);
  lc.clearDisplay(0);
  obrisSrdce(10);
  obrisSrdce(10, false);
}



void loop() { 
  obrisSrdce(10);
  vyplnSrdce(10);
}
