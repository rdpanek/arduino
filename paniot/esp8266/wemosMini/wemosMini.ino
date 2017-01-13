// pouzijeme knihovnu pro praci s displejem
#include "SSD1306.h"

// natahneme fonty, ktere budeme chtit na displeji pouzit
#include "DialogPlain14Bold.h"
#include "DialogPlain12.h"
#include "DialogPlain10.h"

// zavolame konstrukci displeje a uvedeme adresu, ktera je uvedena na zadni strane displeje
// je to poznat podle spoje - cinem
// napr. 0x3d
SSD1306 display(uvedAdresu, D2, D1);


int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;

// nastavime pocitadlo na 0
int i = 0;


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

  // inicializace displeje
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  display.displayOn();

  // nastavime font
  display.setFont(Dialog_bold_14);

  // pripravim string pro vykresleni
  display.drawString(64, displayMarginTop + 25, "0");
  
  // vykreslime
  display.display();
  delay(2000);

  // po2 vterinach vymazeme
  display.clear(); 

  Serial.println("-- start --");
}

void loop() {

  displayMessage(String("pocitadlo"), String(i++), 200);

} 

