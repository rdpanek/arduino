#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#include <SPI.h>
int LED = 7;
int PIR_LED = 8;

// pro mini interrupt pin
int PIR = 2;

Enrf24 radio(9, 10, 6);  // P2.0=CE, P2.1=SCN, P2.2=IRQ

char addr[] = {'p','a','n','i','o','t','r'};
 
void dump_radio_status_to_serialport(uint8_t);
 
void setup() {
  Serial.begin(9600);
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
 
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
 
  radio.setTXaddress((void*)addr);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(PIR_LED, OUTPUT);
  digitalWrite(PIR_LED, LOW);

  //delay(2000);
  attachInterrupt(digitalPinToInterrupt(PIR),sendToGW, CHANGE);
}

void loop() {}

void sendToGW() {  
  radio.lastTXfailed = false;
  int stateOfPir = 0;
  if(digitalRead(PIR) == HIGH){
    stateOfPir = 1;
    digitalWrite(PIR_LED, HIGH);
    delay(100);
    digitalWrite(PIR_LED, LOW);
  } else {
    stateOfPir = 0;
  }
  String message = "pir:g:chodba:";
  message = message + stateOfPir + ":" + ceil(readVcc()/1000);
  Serial.println(message);
  radio.print(message);
  radio.flush();  // Force transmit (don't wait for any more data)
  dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  if(radio.lastTXfailed) {
    digitalWrite(LED, HIGH);
    Serial.println("-- modul nenalezen --");
    delay(100);
    digitalWrite(LED, LOW);
  }
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
