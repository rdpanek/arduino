const int rellayPin = D1;
bool allowRellay = true;
bool stateRellay = false;
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

void handleRellay()
{
  if (stateRellay) {
    autoOffRellay();
    digitalWrite(rellayPin, HIGH);
  } else {
    digitalWrite(rellayPin, LOW);
  }
}
