#include "FastLED.h"

#define NUM_LEDS 1
int BRIGHTNESS = 128;
int MAX_BRIGHTNESS = 128;
bool up = true;

#define DATA_PIN D2

CRGB leds[NUM_LEDS];

void initRGB() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
}

void turnOffRGB() {
  leds[0] = CRGB::Black;
  FastLED.show();  
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
