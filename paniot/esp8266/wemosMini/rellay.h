const int rellayPin = D1;
bool allowRellay = true;
String onEndpoint = "/rellayOn";
String offEndpoint = "/rellayOff";

void rellay(bool state)
{
  if (allowRellay) {
    if (state) {
      digitalWrite(rellayPin, HIGH);
    } else {
       digitalWrite(rellayPin, LOW);
    }
  }
}
