#include <OneWire.h>
#include <DallasTemperature.h>

const int pinCidlaDS = D3;
float dallasTemperature = 0;
int dallasDellayTemperatureMS = 10000;
int dallasLastOnMS = 0;

OneWire oneWireDS(pinCidlaDS);
DallasTemperature dallasDS(&oneWireDS);


// get temperature
//dallasDS.requestTemperatures();
//dallasDS.getTempCByIndex(0)
//measureDallas();

void logDallasToElasticsearch() {
  
  String dallas = "";
  dallas += "{\"location\": \""+deviceLocation+"\"";
  dallas += ",\"heap\":";
  dallas += ESP.getFreeHeap();
  dallas += ",\"rssi\":";
  dallas += WiFi.RSSI();
  dallas += ",\"senzor\":\"temperature\"";
  dallas += ",\"val\":";
  dallas += dallasTemperature;
  dallas += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(dallas);
  dallas = "";
}

void measureDallas() {
  if (millis() > (dallasDellayTemperatureMS + dallasLastOnMS)) { 
    dallasLastOnMS = millis();
    dallasDS.requestTemperatures();
    dallasTemperature = dallasDS.getTempCByIndex(0);
    Blynk.virtualWrite(10, dallasTemperature); // for value
    Blynk.virtualWrite(11, dallasTemperature); // for graph
    logDallasToElasticsearch();
  }
}
