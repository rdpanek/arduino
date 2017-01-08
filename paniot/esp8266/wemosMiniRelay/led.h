const int ledPin = D3;
bool ledIndication = true;
bool allowLed = true;

void ledBlick(int count, int _delay)
{
  if (allowLed) {
    if (ledIndication) {
      for (int _count = 0; _count < count; _count++) {
        digitalWrite(ledPin, HIGH);
        delay(_delay);
        digitalWrite(ledPin, LOW);
        delay(_delay);
      }
    }
  }
}
