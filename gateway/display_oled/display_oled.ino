#include "U8glib.h"
boolean stateOfArm = false;
String carrierName = "Vyhledavam ..."; // max 17 chars
int qualitySignal = 0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup(void) 
{
  Serial.begin(9600);
}

void updateCarrierState(void)
{
  carrierName = "Vodafone CZ";
  carrierName = carrierName.substring(0,17);
  qualitySignal = 25;  
}

void updateStateOfArm(void)
{
  if (stateOfArm == true) stateOfArm = false; else stateOfArm = true;
}

void draw(void) {
  
  // carrier section
  u8g.setFont(u8g_font_6x10);

  // carrier name
  u8g.setPrintPos(0, 13);
  u8g.print(carrierName);

  // quality of signal
  if (qualitySignal != 0)
  {
    u8g.setPrintPos(110, 13);
    u8g.print(qualitySignal);
    u8g.print("%");
  }

  // notice of state
  if (stateOfArm)
  {
    u8g.setColorIndex(1);
    u8g.drawBox(0, 40, 72, 20);
    u8g.setPrintPos(5,55);
    u8g.setFont(u8g_font_courB10);
    u8g.setColorIndex(0);
    u8g.print("AKTIVNI");
    u8g.setColorIndex(1);
  }
}

void loop(void) {
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  // update carrier
  updateCarrierState();
  delay(500);
}
