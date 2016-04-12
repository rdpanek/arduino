// http://hawwwran.launchpad.cz/clanky/transceiver-nrf24l01-zaklady-pouziti-46.html
// arduino pro mini
#include <Enrf24.h>
#include "RF24.h"
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>
#include <EEPROM.h>
int RELAY = 2;
int SPEAKER = 3;
int BUTTON = 4;
int COMMAND_LED = 5;
int NRF_STATUS_LED = 8;
int EEPROM_ADDR_SPEAKER = 1;
int EEPROM_ADDR_DELAY = 2;

// pro mini
Enrf24 radio(9, 10, 6);  // P2.0=CE, P2.1=SCN, P2.2=IRQ

char rxTxAddr[] = {'p','a','n','i','o','t'};
String nrfMessage;
String stateOfNRF24;
String relaySwitch;
boolean stateOfRelay = false;
boolean receiveMode = false;
boolean speaker = false;

boolean reiveMode = false;
long delayOff = 30000; 
long relayJob = 0;

void dump_radio_status_to_serialport(uint8_t);
 
void setup() {
  Serial.begin(9600);
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
 
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());

  radio.setTXaddress((void*)rxTxAddr);
  radio.setRXaddress((void*)rxTxAddr);
  radio.enableRX();
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(COMMAND_LED, OUTPUT);
  digitalWrite(COMMAND_LED, LOW);
  pinMode(NRF_STATUS_LED, OUTPUT);
  digitalWrite(NRF_STATUS_LED, LOW);
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER, LOW);
  pinMode(BUTTON, INPUT);
  relaySwitch = '0';


  if (EEPROM.read(EEPROM_ADDR_SPEAKER) == 255) EEPROM.write(EEPROM_ADDR_SPEAKER,0);
  if (EEPROM.read(EEPROM_ADDR_DELAY) == 255) EEPROM.write(EEPROM_ADDR_DELAY,30); 
}
 
void loop() {
  // po uplynuti nastavene doby sepnuti, vypnout rele
  delayOff = EEPROM.read(EEPROM_ADDR_DELAY);
  delayOff = delayOff * 1000;
  if (millis() > ((relayJob + delayOff) - 10000)) beep();
  if (millis() > (relayJob + delayOff)) {
    stateOfRelay = false; 
    relayJob = 0;
    relaySet();
  }
  
  char inbuf[30];
  dump_radio_status_to_serialport(radio.radioState());  // Should show Receive Mode
  Serial.println(stateOfNRF24);
  //while (!radio.available(true));
  if(radio.available(true) > 0){
    if (radio.read(inbuf)) {
      nrfMessage = inbuf;
      Serial.println(nrfMessage);
      incomingCommand();
      

      // Struktura pro delay / relay:delay:set:10000
      if(getValue(nrfMessage,':',1) == "delay") {
        delayOff = getValue(nrfMessage,':',3).toInt();
        if (delayOff < 10) delayOff = 10;
        EEPROM.write(EEPROM_ADDR_DELAY,delayOff); 
      }

      // Struktura pro speaker / relay:speaker:set:1
      if(getValue(nrfMessage,':',1) == "speaker") {
        if(getValue(nrfMessage,':',3).toInt() == 1) {
          EEPROM.write(EEPROM_ADDR_SPEAKER,1);
        } else {
          EEPROM.write(EEPROM_ADDR_SPEAKER,0);
        }
      }
      
      // Struktura pro rele / relay:relay:set:1
      if(getValue(nrfMessage,':',1) == "relay") {
      
        // zapnuti rele relay:relay:set:1
        // vypnuti rele relay:relay:set:0
        relaySwitch = getValue(nrfMessage,':',3);
        if(relaySwitch.toInt() == 1){
          stateOfRelay = true;
        } else {
          stateOfRelay = false;
        }
        relaySet();
      }
    }
  } else {
    // manualni prepnuti stavu rele
    if(digitalRead(BUTTON) == 1) {
      if(stateOfRelay == true){
        stateOfRelay = false;  
      } else {
        stateOfRelay = true;
      }
      delay(500);
      relaySet();
    }  
  }
}

void beep(){
  if (EEPROM.read(EEPROM_ADDR_SPEAKER) == 1 && speaker == 1) {
    digitalWrite(SPEAKER, HIGH);
    delay(10);
    digitalWrite(SPEAKER, LOW);
    speaker = 0;
  }
}


void relaySet() {
  if(stateOfRelay == true){
    digitalWrite(RELAY, HIGH);
    speaker = 1;
    relayJob = millis();
  } else {
    digitalWrite(RELAY, LOW);  
    relayJob = 0;
  }
}


void incomingCommand() {
  digitalWrite(COMMAND_LED, HIGH);
  delay(10);
  digitalWrite(COMMAND_LED, LOW);
}


void nrfStatusLed(boolean receiveMode = false) {
  if(receiveMode == true) {
    digitalWrite(NRF_STATUS_LED,HIGH);
  } else {
    digitalWrite(NRF_STATUS_LED,LOW);
  }
}
 
void dump_radio_status_to_serialport(uint8_t status)
{
  nrfStatusLed(false);
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      stateOfNRF24 ="NO TRANSCEIVER PRESENT";
      break;
 
    case ENRF24_STATE_DEEPSLEEP:
      stateOfNRF24 = "DEEP SLEEP <1uA power consumption";
      break;
 
    case ENRF24_STATE_IDLE:
      stateOfNRF24 = "IDLE module powered up w/ oscillators running";
      break;
 
    case ENRF24_STATE_PTX:
      stateOfNRF24 = "Actively Transmitting";
      break;
 
    case ENRF24_STATE_PRX:
      nrfStatusLed(true);
      stateOfNRF24 = "Receive Mode";
      break;
 
    default:
      stateOfNRF24 = "UNKNOWN STATUS CODE";
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
