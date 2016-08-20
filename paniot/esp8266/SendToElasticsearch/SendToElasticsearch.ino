/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

String jsonData = "{ \"chip\": \"DS18S20\",\"umisteni\": \"pracovna\",\"teplota\": 23.75,\"freeHeap\": 47656,\"resetReason\": \"External System\",\"chipId\": 8692278,\"flashChipId\": 1327328,\"flashChipSize\": 2097152,\"flashChipRealSize\": 1048576,\"flashChipSpeed\": 40000000,\"vcc\": 3225}";
String returnvalue;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("panek2", "panekSit");

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        http.begin("http://146.148.102.113:9200/wifi-teplomer/wifi-teplomer/"); //HTTP
        http.addHeader("Content-Type","application/json");
        int httpCode = http.POST(jsonData);
        Serial.println(httpCode);
        if (httpCode != 200) {
          Serial.println("not successful");
          } else {
          returnvalue = http.getString();
          Serial.println("successful:" + returnvalue);   
          }
        http.end();
    }

    delay(10000);
}

