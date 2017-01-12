const int ldrPin = 0;
int ldrValue = 0;
int ldrDellayTemperatureMS = 10000;
int ldrLastOnMS = 0;

int getLDR() {
  ldrValue = analogRead(ldrPin);
}

// get value
// measureLDR()


void logLdrToElasticsearch() {
  
  String ldrLog = "";
  ldrLog += "{\"location\": \""+deviceLocation+"\"";
  ldrLog += ",\"heap\":";
  ldrLog += ESP.getFreeHeap();
  ldrLog += ",\"rssi\":";
  ldrLog += WiFi.RSSI();
  ldrLog += ",\"senzor\":\"ldr\"";
  ldrLog += ",\"val\":";
  ldrLog += ldrValue;
  ldrLog += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(ldrLog);
  ldrLog = "";
}

void measureLDR() {
  if (millis() > (ldrDellayTemperatureMS + ldrLastOnMS)) { 
    ldrLastOnMS = millis();
    getLDR();
    logLdrToElasticsearch();
  }
}
