// 3D cube
// Add drawLine to SSD1306.h and SSD1306.cpp : 2016.03.16 macsbug 
// Add WeMos OLED 64x48                      : 2016.03.31 macsbug 
// https://github.com/squix78/esp8266-oled-ssd1306

#include <Wire.h>
#include "SSD1306.h"
#include "DialogPlain14Bold.h"
#include "DialogPlain10.h"
#include "teplotniCidlo.h"

extern "C" {
  #include "user_interface.h"
}
SSD1306 display(0x3c,D2,D1);
int16_t displayMarginLeft = 31;
int16_t displayMarginTop = 13;
char teplota[] = "00.00";
 
void setup() {
  display.init();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.flipScreenVertically();
  display.displayOn();
}
 
void loop() {
  //teplota = String(getTeplota());
  delay(200);
  displayTeplota();
}
 
void displayTeplota() {
  mainFrame();
  display.setFont(Dialog_plain_10);
  display.drawString(displayMarginLeft+12,displayMarginTop+10,"Teplota");
  display.setFont(Dialog_bold_14);
  display.drawString(displayMarginLeft+10,displayMarginTop+25,String(getTeplota()));
  display.display();
  display.clear();
}  

void mainFrame() {
  display.drawRect(displayMarginLeft+1,displayMarginTop+3, 64, 48);  
}
