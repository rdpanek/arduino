const int pirPin = D1;
bool pirState = false;
int _valuePirLog = 0;

int pirDellayMS = 10000;
int pirLastOnMS = 0;

void logPirStateToElasticsearch() {
  if (pirState) {
    _valuePirLog = 1;
  } else {
    _valuePirLog = 0;
  }
  
  String pirLog = "";
  pirLog += "{\"location\": \""+deviceLocation+"\"";
  pirLog += ",\"heap\":";
  pirLog += ESP.getFreeHeap();
  pirLog += ",\"rssi\":";
  pirLog += WiFi.RSSI();
  pirLog += ",\"senzor\":\"pir\"";
  pirLog += ",\"val\":";
  pirLog += _valuePirLog;
  pirLog += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(pirLog);
  pirLog = "";
}

int isPirActive() {
  if(digitalRead(pirPin) == HIGH) {
    pirState = true;
    beep(1,10);
    allowBeep = false;
    if (millis() > (pirDellayMS + pirLastOnMS)) {
      pirLastOnMS = millis();
      sendGetRequest(onEndpoint);
      logPirStateToElasticsearch();
    }
  }  else {
    pirState = false;
    allowBeep = true;
  }
}

// use
// isPirActive()
