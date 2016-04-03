#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>


#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
int TX_FAILED_LED = 3;
volatile int impuls_z_wdt = 1;
volatile int citac_impulsu = 2;
volatile int impulsu_ke_spusteni = 2; // 8 = 1min

#include "DHT.h"
#define DHTPIN 2
#define DHTPIN_POWER 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
 
Enrf24 radio(9, 10, 6);  // P2.0=CE, P2.1=SCN, P2.2=IRQ

char addr[] = {'p','a','n','i','o','t','r'};
 
void dump_radio_status_to_serialport(uint8_t);

ISR(WDT_vect)
{
  if(impuls_z_wdt == 0)
  {
    impuls_z_wdt=1;
  }
}

void setup() {
  Serial.begin(9600);

  dht.begin();
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
 
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
 
  radio.setTXaddress((void*)addr);
  pinMode(TX_FAILED_LED, OUTPUT);
  digitalWrite(TX_FAILED_LED, LOW);
  pinMode(DHTPIN_POWER, OUTPUT);
  digitalWrite(DHTPIN_POWER, LOW);

  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3;
  WDTCSR |= _BV(WDIE);
}
 
void loop() {
  if ((impuls_z_wdt == 1) & (impulsu_ke_spusteni == citac_impulsu))
  {

    digitalWrite(DHTPIN_POWER, HIGH);
    delay(200);
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    Serial.println(temperature);
    if (isnan(humidity) || isnan(temperature)) {
      return;
    }
    digitalWrite(DHTPIN_POWER, LOW);
  
    String message = "temp:g:teplomer:" + String(temperature) + ":" + String(humidity) +":"+ceil(readVcc()/1000);
    
    Serial.println(message);
    radio.lastTXfailed = false;
    radio.print(message);
    radio.flush();
    dump_radio_status_to_serialport(radio.radioState());
    if(radio.lastTXfailed) {
      digitalWrite(TX_FAILED_LED, HIGH);
      Serial.println("-- modul nenalezen --");
      delay(200);
      digitalWrite(TX_FAILED_LED, LOW);
    }
 
    citac_impulsu = 0;
    impuls_z_wdt = 0;
    enterSleep();
  } else {
    enterSleep();
  }
  citac_impulsu++;
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();  
  sleep_mode();
 
  sleep_disable();  
  power_all_enable();
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

