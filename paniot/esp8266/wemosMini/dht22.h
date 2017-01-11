const int dhtPin = D4;

float humidity = 0;
float temperature = 0;
int sht22DellayTemperatureMS = 2000;
int lastOnMS = 0;
float _valueToLog = 0;

#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302)
 
DHT dht(dhtPin, DHTTYPE);

// to setup
// dht.begin();


void logToElasticsearch(String senzorType) {
  if (senzorType == "temperature") {
    _valueToLog = temperature;
  } else {
    _valueToLog = humidity;
  }
  
  String dhtLog = "";
  dhtLog += "{\"location\": \""+deviceLocation+"\"";
  dhtLog += ",\"heap\":";
  dhtLog += ESP.getFreeHeap();
  dhtLog += ",\"rssi\":";
  dhtLog += WiFi.RSSI();
  dhtLog += ",\"senzor\":\""+String(senzorType)+"\"";
  dhtLog += ",\"val\":";
  dhtLog += _valueToLog;
  dhtLog += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(dhtLog);
  dhtLog = "";
}

void measureDht22() {
  if (millis() > (sht22DellayTemperatureMS + lastOnMS)) { 
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
    logToElasticsearch("temperature");
    logToElasticsearch("humidity");
  }
}
