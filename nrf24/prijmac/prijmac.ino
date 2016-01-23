// http://hawwwran.launchpad.cz/clanky/transceiver-nrf24l01-zaklady-pouziti-46.html
// arduino due
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>
#include "U8glib.h"

// mega
Enrf24 radio(40, 53, 41);
// duelaminove 
//Enrf24 radio(9, 10, 6);
//const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
char rxaddr[] = {'g','w','-','0','1'};

String nrfMessage;
String state;
int countOfMessage = 00;
const char *str_on = "ON";
const char *str_off = "OFF";

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
 
void dump_radio_status_to_serialport(uint8_t);
 
void setup() {
  countOfMessage = 0;
  Serial.begin(9600);
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
   
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
 
  radio.setRXaddress((void*)rxaddr);
   
  radio.enableRX();  // Start listening
}

void draw(void) {
  
  // carrier section
  u8g.setFont(u8g_font_6x10);

  // carrier name
  u8g.setPrintPos(0, 13);
  u8g.print(nrfMessage);

  // carrier name
  u8g.setPrintPos(0, 26);
  u8g.print("Count: ");
  u8g.print(countOfMessage);

  
  u8g.setPrintPos(0, 36);
  u8g.print(state);
  
}
 
void loop() {
  delay(200);
  
  char inbuf[33];
   
  dump_radio_status_to_serialport(radio.radioState());  // Should show Receive Mode

 Serial.println(state);
  while (!radio.available(true));
  if (radio.read(inbuf)) {
    nrfMessage = inbuf;
    Serial.println(nrfMessage);
    ++countOfMessage;
  }
}
 
void dump_radio_status_to_serialport(uint8_t status)
{
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      state ="NO TRANSCEIVER PRESENT";
      break;
 
    case ENRF24_STATE_DEEPSLEEP:
      state = "DEEP SLEEP <1uA power consumption";
      break;
 
    case ENRF24_STATE_IDLE:
      state = "IDLE module powered up w/ oscillators running";
      break;
 
    case ENRF24_STATE_PTX:
      state = "Actively Transmitting";
      break;
 
    case ENRF24_STATE_PRX:
      state = "Receive Mode";
      break;
 
    default:
      state = "UNKNOWN STATUS CODE";
  }
}
