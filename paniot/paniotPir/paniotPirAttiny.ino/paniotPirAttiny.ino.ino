/**
 * A Mirf example test sending and receiving data between
 * Ardunio (running as server) and ATTiny85 (running as client).
 *
 * This uses the SPI85 class from: https://github.com/stanleyseow/attiny-nRF24L01/tree/master/libraries/SPI85
 *
 * Pins:
 * Hardware SPI:
 * MISO -> PB0
 * MOSI -> PB1
 * SCK -> PB2
 *
 * (Configurable):
 * CE -> PB4
 * CSN -> PB3
 */

#include <SPI85.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpi85Driver.h>
String message = "relay:relay:set:1";
int LED = 5;

void setup() {
  /*
   * Setup pins / SPI.
   */

  Mirf.cePin = PB4;
  Mirf.csnPin = PB3;

  Mirf.spi = &MirfHardwareSpi85;
  Mirf.init();
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
  Mirf.channel = 0; 
  Mirf.config();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  Mirf.setTADDR((byte *)"paniot");
  
  Mirf.send((byte*)&message);

  digitalWrite(LED, HIGH);
  delay(10);
  digitalWrite(LED, LOW);
  delay(1000);
} 
  
  
  
