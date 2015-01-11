 #include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

void draw(void) 
{
  u8g.drawFrame(0,0,128,33);
  u8g.setFont(u8g_font_6x10);
  u8g.setPrintPos(4, 13);
  u8g.print("udalost: 10/10 00:45");
  u8g.setPrintPos(4, 25);
  u8g.print("cidlo: obyvak");

  u8g.setColorIndex(1);
  u8g.drawBox(103, 60, 6, 2);
  u8g.drawBox(111, 57, 6, 5);
  u8g.drawBox(119, 52, 6, 10);
  u8g.drawBox(0, 42, 90, 20);
  u8g.setColorIndex(0);
  u8g.setPrintPos(8,58);
  u8g.setFont(u8g_font_fub11);
  u8g.print("POPLACH");

  u8g.setColorIndex(1);

}

void setup(void) 
{
  // flip screen, if required
  // u8g.setRot180();
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(500);
}
