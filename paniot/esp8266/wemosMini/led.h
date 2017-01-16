const int ledPin = D8;

void ledBlick(int count, int _delay)
{
  for (int _count = 0; _count < count; _count++) {
    digitalWrite(ledPin, HIGH);
    delay(_delay);
    digitalWrite(ledPin, LOW);
    delay(_delay);
  }
}
