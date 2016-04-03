#include "LowPower.h"
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
int PERIFER = 4;

DHT dht(DHTPIN, DHTTYPE);
Enrf24 radio(9, 10, 6);  // P2.0=CE, P2.1=SCN, P2.2=IRQ
char addr[] = {'p','a','n','i','o','t','r'};

void setup() {
  Serial.begin(9600);
  dht.begin();
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
  pinMode(PERIFER, OUTPUT);
  digitalWrite(PERIFER, LOW);

  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  radio.setTXaddress((void*)addr);
}

void loop() {
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  digitalWrite(PERIFER, HIGH);
  delay(250);

  float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      return;
    }
  
    String message = "temp:g:teplomer:" + String(temperature) + ":" + String(humidity) +":"+ceil(readVcc()/1000);
    
    Serial.println(message);
    radio.lastTXfailed = false;
    radio.print(message);
    radio.flush();
    if(radio.lastTXfailed) {
      Serial.println("-- modul nenalezen --");
    }
    dump_radio_status_to_serialport(radio.radioState());
    delay(250);
  
  digitalWrite(PERIFER, LOW);
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

long readVcc() {
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  uint8_t low = ADCL;
  uint8_t high = ADCH;
  long result = (high<<8) | low;
  result = 1125300L / result; // Výpočet Vcc (mV); 1125300 = 1.1*1023*1000
  return result;
}
