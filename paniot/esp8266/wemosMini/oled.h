// oled display
#include "SSD1306.h"
#include "DialogPlain14Bold.h"
#include "DialogPlain12.h"
#include "DialogPlain10.h"

SSD1306 display(0x3c, 2, 1);
int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;


void oledInit() {
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  display.displayOn();

  display.setFont(Dialog_bold_14);
  display.drawString(64, displayMarginTop + 25, "PanIoT");
  display.display();
  delay(2000);
  display.clear();  
}

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
