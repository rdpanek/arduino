const int rellayPin = D1;
bool allowRellay = true;

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
