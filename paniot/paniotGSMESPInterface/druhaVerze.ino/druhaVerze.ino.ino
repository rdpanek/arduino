   
#include "U8glib.h"  
#include <SoftwareSerial.h>
SoftwareSerial SIM900(3, 2);
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7);
   
void u8g_prepare() 
{  
  u8g.setFont(u8g_font_6x10);  
  u8g.setFontRefHeightExtendedText();  
  u8g.setDefaultForegroundColor();  
  u8g.setFontPosTop();  
}  
   
void helloDisplay()
{
  u8g.setFont(u8g_font_9x18B);  
  u8g.drawStr( 11, 15, "PanIoT GSM");   
  u8g.drawFrame(0,0,128,64);   
}  


void draw() 
{
  helloDisplay();
}
   
void setup() 
{  
  Serial.begin(19200);
  SIM900.begin(19200);
  delay(1000);

  //u8g.setRot180();  
  //u8g.setColorIndex(1);
}  
   
void loop() 
{  
  u8g.firstPage();   
  do 
  {  
  draw();  
  } 
  while( u8g.nextPage() );  
  delay(3000); 
  SIM900.println("AT");
  delay(3000);
  if(Serial.available())
    SIM900.println(Serial.read());
  if (SIM900.available())
    Serial.write(SIM900.read());
}    
