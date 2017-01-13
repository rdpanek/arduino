#include "SSD1306.h"
#include "DialogPlain14Bold.h"
#include "DialogPlain12.h"
#include "DialogPlain10.h"

SSD1306 display(0x3c, D2, D1);


int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;
const int ledPin = D8;
const int ldrPin = 0;
int ldrValue = 0;

void mainFrame() {
  display.drawRect(displayMarginLeft + 1, displayMarginTop + 3, 64, 48);
}

void displayMessage(String firstLine, String secondLine, int _delay) {
  mainFrame();
  display.setFont(Dialog_plain_10);
  display.drawString(64, displayMarginTop + 10, firstLine);
  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, secondLine);
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
  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, "0");
  display.display();
  delay(2000);
  display.clear(); 

  Serial.println("-- start --");
}

void loop() {

  ldrValue = analogRead(ldrPin);

  if (ldrValue < 500) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  displayMessage(String("LDR"), String(ldrValue), 200);

} 

