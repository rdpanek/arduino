#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <string.h>

float celsius;
ADC_MODE(ADC_VCC);
OneWire  ds(2);  // on pin 2 (a 4.7K resistor is necessary)
ESP8266WiFiMulti WiFiMulti;

String jsonData;
String dsoChip;

int LED = 4;

String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }

  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  WiFiMulti.addAP("panek2", "panekSit");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      dsoChip = "DS18S20";
      type_s = 1;
      break;
    case 0x28:
      dsoChip = "DS18B20";
      type_s = 0;
      break;
    case 0x22:
      dsoChip = "DS1822";
      type_s = 0;
      break;
    default:
      dsoChip = "Device is not a DS18x20 family device.";
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;

  jsonData += "{ \"chip\": \""+dsoChip+"\"";
  jsonData += ",\"umisteni\": \"pracovna\"";
  jsonData += ",\"teplota\": \"";
  char tmp[5]; 
  dtostrf(celsius, 1, 2, tmp);
  jsonData += tmp;
  jsonData += "\"";
  jsonData += ",\"freeHeap\": "+String(ESP.getFreeHeap());
  jsonData += ",\"resetReason\": \""+ESP.getResetReason()+"\"";
  jsonData += ",\"flashChipSpeed\": "+String(ESP.getFlashChipSpeed());
  jsonData += ",\"vcc\":"+String(ESP.getVcc());
  jsonData += ",\"dateTime\":\""+String(getTime())+"\"";
  jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
  jsonData += "}";

  Serial.println(jsonData);
  // wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("http://146.148.102.113:9200/wifi-teplomer/wifi-teplomer/"); //HTTP
    http.addHeader("Content-Type","application/json; charset=UTF-8");
    int httpCode = http.POST(jsonData);
    if (httpCode < 300) {
      digitalWrite(LED, HIGH);
      Serial.println(http.getString());
      delay(100);
      digitalWrite(LED, LOW);
    } else { 
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
    }
    Serial.println(http.getString());
    http.end();
  }
  jsonData = "";
  delay(60000);
}

