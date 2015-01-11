/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>



//
// Hardware configuration
//

// External startup function
// Set up nRF24L01 radio on SPI bus plus pins 7 & 6
// (This works for the Getting Started board plugged into the
// Maple Native backwards.)

RF24 radio(8,9);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 2000; //ms

// When did we last send?
unsigned long last_sent;

void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent > interval  )
  {
    last_sent = now;

    Serial.println("Sending...\r\n");
    const char* hello = "Hello, world!";
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,hello,strlen(hello));
    if (ok)
      Serial.println("\tok.\r\n");
    else
    {
      Serial.println("\tfailed.\r\n");
      delay(250); // extra delay on fail to keep light on longer
    }
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
