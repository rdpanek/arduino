const int pirPin = D1;
bool pirState = false;

int isPirActive() {
  if(digitalRead(pirPin) == HIGH) {
    pirState = true;
    beep(1,10);
    allowBeep = false;
  }  else {
    pirState = false;
  }
}

// use
// isPirActive()
