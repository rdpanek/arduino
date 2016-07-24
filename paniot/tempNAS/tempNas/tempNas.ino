#include <OneWire.h>
OneWire  ds(2);  // on pin 2 (a 4.7K resistor is necessary)
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

int fan = 10;
int fanSpeed;
float celsius;

void setup(void) {
  Serial.begin(9600);
  pinMode(fan, OUTPUT);
}

void loop(void) {

  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  Serial.println(celsius);

  if (celsius < 16 ) { fanSpeed = 50; }
  if (celsius > 16 ) { fanSpeed = 80; }
  if (celsius > 20 ) { fanSpeed = 100; }
  if (celsius > 22 ) { fanSpeed = 150; }
  if (celsius > 24 ) { fanSpeed = 200; }
  if (celsius > 30 ) { fanSpeed = 255; }
  Serial.println(fanSpeed);
  analogWrite(fan, fanSpeed);
}

void draw(void) {
  
  // carrier section
  u8g.setFont(u8g_font_6x10);

  // carrier name
  u8g.setPrintPos(0, 13);
  u8g.setFont(u8g_font_courB10);
  u8g.print("Temp:");
  u8g.setPrintPos(50, 13);
  u8g.print(celsius);

  u8g.setPrintPos(0, 50);
  u8g.setFont(u8g_font_courB10);
  u8g.print("Fan: ");
  u8g.setPrintPos(60, 50);
  u8g.print(fanSpeed);
}
