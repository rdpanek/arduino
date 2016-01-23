/*
 * light_ws2812 example
 *
 * Created: 07.03.2014 12:49:30
 *  Author: Matthias Riegler
 */ 

#include <WS2812.h>

WS2812 LED(8); // 1 LED
  
cRGB value;

void setup() {
  LED.setOutput(9); // Digital Pin 9
}

void loop() {

  value.b = 21; value.g = 255; value.r = 4;
  LED.set_crgb_at(7, value);
  LED.sync();
  delay(30);


  value.b = 0; value.g = 0; value.r = 0;
  LED.set_crgb_at(0, value);
  LED.set_crgb_at(1, value);
  LED.set_crgb_at(2, value);
  LED.set_crgb_at(3, value);
  LED.set_crgb_at(4, value);
  LED.set_crgb_at(5, value);
  LED.set_crgb_at(6, value);
  LED.set_crgb_at(7, value);
  LED.sync();
  delay(5000);

}

