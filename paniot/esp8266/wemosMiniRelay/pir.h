const int pirPin = D1;
bool pirState = false;

int isPirActive() {
  if(digitalRead(pirPin) == HIGH) {
    pirState = true;
    ledBlick(1,10);
    allowLed = false;
    beep(1,10);
    allowBeep = false;
  }  else {
    allowLed = true;
    allowBeep = true;
    pirState = false;
  }
}

// use
// isPirActive()
