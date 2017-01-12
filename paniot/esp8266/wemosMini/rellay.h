const int rellayPin = D1;
bool allowRellay = true;
bool stateRellay = false;
bool lastStateRellay = false;
String onEndpoint = "/rellayOn";
String offEndpoint = "/rellayOff";
int rellayDellayMS = 10000;
int rellayLastOnMS = 0;
bool rellayAllowAutoOff = true;

void autoOffRellay() {
  if (rellayAllowAutoOff) {
    if (millis() > (rellayDellayMS + rellayLastOnMS)) { 
      stateRellay = false;
    }  
  }
}

void logStateToElasticsearch() {
  if (stateRellay) {
    _valueToLog = 1;
  } else {
    _valueToLog = 0;
  }
  
  String dhtLog = "";
  dhtLog += "{\"location\": \""+deviceLocation+"\"";
  dhtLog += ",\"heap\":";
  dhtLog += ESP.getFreeHeap();
  dhtLog += ",\"rssi\":";
  dhtLog += WiFi.RSSI();
  dhtLog += ",\"senzor\":\"rellay\"";
  dhtLog += ",\"val\":";
  dhtLog += _valueToLog;
  dhtLog += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(dhtLog);
  dhtLog = "";
}

void handleRellay()
{
  if (stateRellay) {
    autoOffRellay();
    digitalWrite(rellayPin, HIGH);
  } else {
    digitalWrite(rellayPin, LOW);
  }

  if ( lastStateRellay != stateRellay) {
    logStateToElasticsearch();
    lastStateRellay = stateRellay;
  }
}
