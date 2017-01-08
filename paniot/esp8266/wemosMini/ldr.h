const int ldrPin = 0;
int ldrValue = 0;

int getLDR() {
  ldrValue = analogRead(ldrPin);
  return ldrValue;
}

// get value
// getLDR()
