/*
  Demo for RF remote switch receiver.
  For details, see RemoteReceiver.h!

  This sketch shows the received signals on the serial port.
  Connect the receiver to digital pin 2.
*/

#include <RemoteReceiver.h>
#include <WS2812.h>

WS2812 LED(1); // 1 LED
        
cRGB value;

boolean startBuzz = false;

// melody
#include "starWarsSound.h"
void setup() {
  Serial.begin(115200);

  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 3 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  RemoteReceiver::init(0, 3, showCode);

  //pinMode(BUZZ, OUTPUT);
  //digitalWrite(BUZZ, LOW);

  initStarWars();
  LED.setOutput(9);
  greenLed();
}

void loop() {
  if ( startBuzz == true ) beep();
}

// Callback function is called only when a valid code is received.
void showCode(unsigned long receivedCode, unsigned int period) {
  Serial.println(receivedCode);
  beep();
}

void beep() {
  redLed();
  playStarWars();
  startBuzz = false;
  greenLed();
}

void greenLed() {
  value.b = 50; value.g = 205; value.r = 50; // RGB Value -> Red
  LED.set_crgb_at(0, value); // Set value at LED found at index 0
  LED.sync(); // Sends the value to the LED  
  delay(200);
  value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
  LED.set_crgb_at(0, value); // Set value at LED found at index 0
  LED.sync(); // Sends the value to the LED  
}

void redLed() {
  value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
  LED.set_crgb_at(0, value); // Set value at LED found at index 0
  LED.sync(); // Sends the value to the LED 
  delay(200);
  value.b = 255; value.g = 0; value.r = 0; // RGB Value -> Red
  LED.set_crgb_at(0, value); // Set value at LED found at index 0
  LED.sync(); // Sends the value to the LED
}

