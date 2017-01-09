const int dhtPin = D4;

float humidity = 0;
float temperature = 0;
int dellayTemperatureMS = 2000;
int lastOnMS = 0;

#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302)
 
DHT dht(dhtPin, DHTTYPE);

// to setup
// dht.begin();


void logTemperatureToElasticsearch() {
  String jsonData = "";
  jsonData += "{\"location\": \""+deviceLocation+"\"";
  jsonData += ",\"senzor\": \"dht22Temperature\"";
  jsonData += ",\"val\":"+String(temperature);
  jsonData += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";

  sendToElasticsearch(jsonData);
  jsonData = "";
}

void logHumidityToElasticsearch() {
  String jsonData = "";
  jsonData += "{\"location\": \""+deviceLocation+"\"";
  jsonData += ",\"senzor\": \"dht22Humidity\"";
  jsonData += ",\"val\":"+String(humidity);
  jsonData += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";

  sendToElasticsearch(jsonData);
  jsonData = "";
}

void measureDht22() {
  if (millis() > (dellayTemperatureMS + lastOnMS)) { 
    lastOnMS = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
   
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
    humidity = h;
    temperature = t;
    logTemperatureToElasticsearch();
    logHumidityToElasticsearch();
  }
}
