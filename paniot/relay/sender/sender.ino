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
#define RED     0xF800
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
char txaddr[] = {'p','a','n','i','o','t'};
char rxaddr[] = {'p','a','n','i','o','t','r'};

boolean modulState;
String state;
char inbuf[100];
String nrfMessage;
String temperature = "00.00";
String humidity = "00.00";
String voltageTemperature = "00.00";
String voltageOfPir = "0.00";
int stateOfNrf24 = 0;
int stateOfRelay = 0;
int stateOfPir = 0;
int lastTXfailed = 0;

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
 
  radio.setTXaddress((void*)txaddr);
  radio.setRXaddress((void*)rxaddr);
  radio.enableRX();
}

unsigned long draw() {

  tft.setRotation(3);

  // gsm / nrf state
  tft.setCursor(4, 8);
  tft.setTextColor(WHITE);
  tft.print("T-Mobile CZ  |  20%  | 21:55");

  // trigger
  tft.drawRect(2, 24, 100, 80, WHITE);
  tft.drawRect(104, 24, 100, 80, WHITE);
  tft.drawRect(206, 24, 100, 80, WHITE);

  nrf24StatusIco();

    // temperature
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(1);
    tft.setCursor(6, 30);
    tft.print("C: " + temperature);
    tft.setCursor(6, 40);
    tft.print("%: " + humidity);
    tft.setCursor(6, 50);
    tft.print("V: " + voltageTemperature);
    tft.setCursor(6, 70);
    tft.print("-- templomer --");

    // stateOfPir
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(1);
    tft.setCursor(108, 30);
    tft.print("PIR ---->");
    if (stateOfPir == 1) {
      tft.drawRect(108, 50, 30, 30, GREEN);
      tft.fillRect(108, 50, 30, 30, GREEN);
    } else {
      tft.drawRect(108, 50, 30, 30, LIGHTGREY);
      tft.fillRect(108, 50, 30, 30, LIGHTGREY);
    }
    tft.setCursor(108, 90);
    tft.print("V: " + voltageOfPir);

    // stateOfRelay
    tft.setTextColor(WHITE,BLACK);
    tft.setTextSize(1);
    tft.setCursor(210, 30);
    tft.print("----> Relay");
    if (stateOfRelay == 1) {
      tft.drawRect(210, 50, 30, 30, GREEN);
      tft.fillRect(210, 50, 30, 30, GREEN);
    } else {
      tft.drawRect(210, 50, 30, 30, LIGHTGREY);
      tft.fillRect(210, 50, 30, 30, LIGHTGREY);
    }
}
 
void loop() {
  dump_radio_status_to_serialport(radio.radioState());
  draw();

  // relay
  radio.lastTXfailed = false;
  if (stateOfPir) {
    String relaySetSwitchCommand = "relay:relay:set:1";
    Serial.println(relaySetSwitchCommand);
    radio.print(relaySetSwitchCommand);
    radio.flush();  // Force transmit (don't wait for any more data)
    if(radio.lastTXfailed) {
      modulState = false;
      lastTXfailed = 0;
      stateOfRelay = 0;
    } else {
      modulState = true;
      lastTXfailed = 1;
      stateOfRelay = 1;
    }
  }

  if(radio.available(true) > 0){
    if (radio.read(inbuf)) {
      nrfMessage = inbuf;
      Serial.println(inbuf);

      // pir:g:chodba:1
      if(getValue(nrfMessage,':',0) == "pir" && getValue(nrfMessage,':',1) == "g") {
        stateOfPir = getValue(nrfMessage,':',3).toInt();
        voltageOfPir = getValue(nrfMessage,':',4);
        if (stateOfPir == 0) {
          stateOfRelay = 0;
        }
      }

      // temp:g:teplomer:7.00:29.00:5,4V
      if(getValue(nrfMessage,':',0) == "temp" && getValue(nrfMessage,':',1) == "g") {
        temperature = getValue(nrfMessage,':',3);
        humidity = getValue(nrfMessage,':',4);
        voltageTemperature = getValue(nrfMessage,':',5);
      }
    }
  }
  dump_radio_status_to_serialport(radio.radioState());
  draw();
}
 
void dump_radio_status_to_serialport(uint8_t status)
{
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      Serial.println("NO TRANSCEIVER PRESENT");
      stateOfNrf24 = 0;
      break;
 
    case ENRF24_STATE_DEEPSLEEP:
      Serial.println("DEEP SLEEP <1uA power consumption");
      stateOfNrf24 = 1;
      break;
 
    case ENRF24_STATE_IDLE:
      Serial.println("IDLE module powered up w/ oscillators running");
      stateOfNrf24 = 3;
      break;
 
    case ENRF24_STATE_PTX:
      Serial.println("Actively Transmitting");
      stateOfNrf24 = 2;
      break;
 
    case ENRF24_STATE_PRX:
      Serial.println("Receive Mode");
      stateOfNrf24 = 1;
      break;
 
    default:
      state = "UNKNOWN STATUS CODE";
  }
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void nrf24StatusIco() {

  int icoX = 300;
  int icoY = 8;

  // tx status
  if(lastTXfailed == 1) {
    tft.drawTriangle(icoX, icoY, icoX + 6, icoY, icoX + 3, icoY - 3, GREEN);
    tft.fillTriangle(icoX, icoY, icoX + 6, icoY, icoX + 3, icoY - 3, GREEN);
  } else {
    tft.drawTriangle(icoX, icoY, icoX + 6, icoY, icoX + 3, icoY - 3, RED);
    tft.fillTriangle(icoX, icoY, icoX + 6, icoY, icoX + 3, icoY - 3, RED);
  } 
  
  // rx status
  if(stateOfNrf24 == 1){
    tft.drawTriangle(icoX, icoY + 4, icoX + 6, icoY + 4, icoX + 3, icoY + 7, GREEN);
    tft.fillTriangle(icoX, icoY + 4, icoX + 6, icoY + 4, icoX + 3, icoY + 7, GREEN);
  } else {
    tft.drawTriangle(icoX, icoY + 4, icoX + 6, icoY + 4, icoX + 3, icoY + 7, RED);
    tft.fillTriangle(icoX, icoY + 4, icoX + 6, icoY + 4, icoX + 3, icoY + 7, RED);
  } 
}
