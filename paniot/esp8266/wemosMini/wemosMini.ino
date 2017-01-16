
#include "SSD1306.h"

// custom font generator http://oleddisplay.squix.ch/#/home
#include "Irish_Growler_20.h"
#include "Smokum_26.h"


#include <OneWire.h>
#include <DallasTemperature.h>

SSD1306 display(0x3c, D2, D1);

int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;
const int pinCidlaDS = D3;
float dallasTemperature = 0;

OneWire oneWireDS(pinCidlaDS);
DallasTemperature dallasDS(&oneWireDS);


void displayMessage(String text, int _delay) {
  display.setFont(Smokum_26);
  display.drawString(64, displayMarginTop + 15, text);
  display.display();
  delay(_delay);
  display.clear();
}

void setup() {
  Serial.begin(115200);

  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  display.displayOn();
  display.drawString(64, displayMarginTop + 15, "Wi-Fi");
  display.display();
  delay(2000);
  display.clear(); 

  Serial.println("-- start --");
}

void loop() {
  dallasDS.requestTemperatures();
  dallasTemperature = dallasDS.getTempCByIndex(0);
  displayMessage(String((int)dallasTemperature)+" °C", 200);

  delay(1000);
} 

