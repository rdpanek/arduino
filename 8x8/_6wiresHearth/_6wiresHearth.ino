int clockPin = 10;
int latchPin = 9;
int dataPin = 8;

int clockPinx = 13;
int latchPinx = 12;
int dataPinx = 11;

int potPin = 2;
int val = 0;

void setup() {
  pinMode(latchPinx, OUTPUT);
  pinMode(clockPinx, OUTPUT);
  pinMode(dataPinx, OUTPUT);
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  reset();
  delay(500);
  obvodSrdce();
    
}

void loop() {
  velkeSrdce();
}

void reset() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, B11111111);  
  digitalWrite(latchPin, HIGH);  
  delay(500);
}

void velkeSrdce() {
  byte ledStates[8] = {B00000000, B00011000, B00111100, B01111110, B11111111, B11111111, B11111111, B01100110};
  byte GroundLEDs [8] = {B11111110, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B01111111};
  for(int i=0;i<sizeof(ledStates);i++)
  {
    SetStates(ledStates[i]);
    GroundCorrectLED (GroundLEDs[i]);
  }
}

void obvodSrdce() {
  byte ledStates[9] = {B00000000, B00011000, B00100100, B01000010, B10000001, B10000001, B01000010, B00100100, B00011000};
  byte GroundLEDs [9] = {B11111111, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B10111111, B11011111};
  for(int i=0;i<sizeof(ledStates);i++)
  {
    SetStates(ledStates[i]);
    GroundCorrectLED (GroundLEDs[i]);
    delay(100);
  }
}

void GroundCorrectLED (byte states){
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, states);  
    digitalWrite(latchPin, HIGH);
    
}

void SetStates (byte statesx){
       
    digitalWrite(latchPinx, LOW);
    shiftOut(dataPinx, clockPinx, LSBFIRST, statesx);  
    digitalWrite(latchPinx, HIGH);
    
     }
     


