const int rellayPin = D1;
bool allowRellay = true;
bool stateRellay = false;
String onEndpoint = "/rellayOn";
String offEndpoint = "/rellayOff";
int dellayMS = 10000;
int lastOnMS = 0;
bool allowAutoOff = true;

void autoOffRellay() {
  if (allowAutoOff) {
    if (millis() > (dellayMS + lastOnMS)) { 
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
