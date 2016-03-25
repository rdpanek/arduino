#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>
int LED = 3;

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
 
Enrf24 radio(9, 10, 6);  // P2.0=CE, P2.1=SCN, P2.2=IRQ

char addr[] = {'p','a','n','i','o','t','r'};
const char *str_off = "OFF";
 
void dump_radio_status_to_serialport(uint8_t);
 
void setup() {
  Serial.begin(9600);

  dht.begin();
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
 
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
 
  radio.setTXaddress((void*)addr);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}
 
void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  String message = "temp:g:teplomer:" + String(temperature) + ":" + String(humidity) +":5.4";
  
  Serial.print("Sending packet: ");
  Serial.println(message);
  radio.lastTXfailed = false;
  radio.print(message);
  radio.flush();  // Force transmit (don't wait for any more data)
  dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  if(radio.lastTXfailed) {
    digitalWrite(LED, HIGH);
    Serial.println("-- modul nenalezen --");
    delay(200);
    digitalWrite(LED, LOW);
  }
  delay(30000);
}
 
void dump_radio_status_to_serialport(uint8_t status)
{
  Serial.print("Enrf24 radio transceiver status: ");
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      Serial.println("NO TRANSCEIVER PRESENT");
      break;
 
    case ENRF24_STATE_DEEPSLEEP:
      Serial.println("DEEP SLEEP <1uA power consumption");
      break;
 
    case ENRF24_STATE_IDLE:
      Serial.println("IDLE module powered up w/ oscillators running");
      break;
 
    case ENRF24_STATE_PTX:
      Serial.println("Actively Transmitting");
      break;
 
    case ENRF24_STATE_PRX:
      Serial.println("Receive Mode");
      break;
 
    default:
      Serial.println("UNKNOWN STATUS CODE");
  }
}
