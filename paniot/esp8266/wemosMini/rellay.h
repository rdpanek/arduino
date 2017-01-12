const int rellayPin = D1;
bool allowRellay = true;
bool stateRellay = false;
bool lastStateRellay = false;
String onEndpoint = "/rellayOn";
String offEndpoint = "/rellayOff";
int rellayDellayMS = 10000;
int rellayLastOnMS = 0;
bool rellayAllowAutoOff = true;
int _valueToLog = 0;

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
  
  String rellayLog = "";
  rellayLog += "{\"location\": \""+deviceLocation+"\"";
  rellayLog += ",\"heap\":";
  rellayLog += ESP.getFreeHeap();
  rellayLog += ",\"rssi\":";
  rellayLog += WiFi.RSSI();
  rellayLog += ",\"senzor\":\"rellay\"";
  rellayLog += ",\"val\":";
  rellayLog += _valueToLog;
  rellayLog += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(rellayLog);
  rellayLog = "";
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
