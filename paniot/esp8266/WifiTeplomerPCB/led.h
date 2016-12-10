const int ledPin = 15;
bool ledIndication = true;

void blick(int count, int _delay)
{
  if (ledIndication) {
    for (int _count = 0; _count < count; _count++) {
      digitalWrite(ledPin, HIGH);
      delay(_delay);
      digitalWrite(ledPin, LOW);
      delay(_delay);
    }
  }
}
