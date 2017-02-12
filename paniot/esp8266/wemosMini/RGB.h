#include "FastLED.h"

#define NUM_LEDS 1
int BRIGHTNESS = 30;
bool up = true;

#define DATA_PIN D2

CRGB leds[NUM_LEDS];

void turnOffRGB() {
  leds[0] = CRGB::Black;
  FastLED.show();  
}

void initRGB() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  turnOffRGB();
}

void ledBlick(int count, int _delay, String color)
{
  FastLED.setBrightness(  BRIGHTNESS );
  
  for (int _count = 0; _count < count; _count++) {
     if ( color == "blue") { leds[0] = CRGB::Red; }
      if ( color == "green") { leds[0] = CRGB::Blue; }
      if ( color == "white") { leds[0] = CRGB::White; }
      if ( color == "red") { leds[0] = CRGB::Green; }
      FastLED.show(); 
      delay(_delay);
      leds[0] = CRGB::Black;
      FastLED.show(); 
      delay(_delay);
    }
}


void pulseRGB() { 

  FastLED.setBrightness(  BRIGHTNESS );
  //leds[0] = CRGB::Red; // blue
  //leds[0] = CRGB::Blue; // green
  //leds[0] = CRGB::White; // white
  //leds[0] = CRGB::Green; // red
  leds[0] = CRGB::White;
  FastLED.show();
  delay(10);

  /*
  if (up) {
    BRIGHTNESS++; 
  } else {
    BRIGHTNESS--;
  }

  if (BRIGHTNESS == 128) {
    up = false; 
  } else if (BRIGHTNESS == 0) {
    up = true;
  }
  */

}
