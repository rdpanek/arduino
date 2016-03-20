// http://hawwwran.launchpad.cz/clanky/transceiver-nrf24l01-zaklady-pouziti-46.html
// arduino due
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xFFFF
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTGREY 0xC618

#define color = GREEN;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// mega
Enrf24 radio(40, 53, 41);
// duelaminove 
//Enrf24 radio(9, 10, 6);
//const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
char rxaddr[] = {'p','a','n','i','o','t'};

boolean modulState;
String state;
int stateOfRelay = 0;
String relaySetSwitchCommand = "relay:relay:set:";
 
void dump_radio_status_to_serialport(uint8_t);
 
void setup() {

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  tft.fillScreen(BLACK);

  Serial.begin(9600);
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
   
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
 
  radio.setTXaddress((void*)rxaddr);
}

unsigned long draw() {

  tft.setRotation(3);
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Ovladani svetel:");

  tft.setCursor(10, 30);
  tft.setTextColor(YELLOW, BLACK);  tft.setTextSize(1);
  tft.print(relaySetSwitchCommand+stateOfRelay);

  tft.setCursor(10, 50);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("Modul: ");
  tft.setTextColor(BLUE, BLACK);
  if(modulState == true) {
    tft.print("nalezen");  
    Serial.println("nalezen");
  } else {
    tft.print("nenalezen");
    Serial.println("NEnalezen");
  }
  
  tft.setCursor(10, 70);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("Mod modulu: ");
  tft.setTextColor(BLUE, BLACK);
  tft.print(state);
}
 
void loop() {
  draw();
  Serial.print("Sending packet: ");
  Serial.println(relaySetSwitchCommand+stateOfRelay);
  radio.lastTXfailed = false;
  radio.print(relaySetSwitchCommand+stateOfRelay);
  radio.flush();  // Force transmit (don't wait for any more data)
  dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  if(radio.lastTXfailed) {
    modulState = false;
  } else {
    modulState = true;
  }

  if(stateOfRelay == 1){
    stateOfRelay = 0;  
  } else {
    stateOfRelay = 1;
  }
  
  delay(1000);

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
