const int beepPin = D0;
bool beepOn = true;
bool allowBeep = true;

void beep(int count, int _delay)
{
  if (allowBeep) {
    if (beepOn) {
      for (int _count = 0; _count < count; _count++) {
        digitalWrite(beepPin, HIGH);
        delay(_delay);
        digitalWrite(beepPin, LOW);
        delay(_delay);
      }
    }
  }
}
