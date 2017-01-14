const int multiLed1 = D8;
const int multiLed2 = D7;
const int multiLed3 = D6;
const int multiLed4 = D5;
bool allowLed = true;


void ledOn(bool allowOn, int ledPosition) {
  if (allowOn) digitalWrite(ledPosition, HIGH);
  if (!allowOn) digitalWrite(ledPosition, LOW);
}

void ledBlick(int count, int _delay, int ledPosition)
{
  if (allowLed) {
    for (int _count = 0; _count < count; _count++) {
      digitalWrite(ledPosition, HIGH);
      delay(_delay);
      digitalWrite(ledPosition, LOW);
      delay(_delay);
    }
  }
}
