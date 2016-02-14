// uno
//int clockPin = 12; // MISO
//int latchPin = 8; // CLKO
//int dataPin = 11; // MOSI

// atiny
int clockPin = 1; // MISO
int latchPin = 4; // CLKO
int dataPin = 0; // MOSI

// 595
// DS 14 = dataPin
// ST_CP 12 = clockPin
// SH_CP 11 = latchPin

int potPin = 2;
int val = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  reset();
  obvodSrdce();
  
}

void loop() {
  srdce();
}

void reset() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, B00000000);  
  digitalWrite(latchPin, HIGH);  
}

void obvodSrdce() {
  int count = 10;
  while(count>0) {
    byte ledStates[17] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111101, B11111011, B11111011, B11111101, B11111101, B11111011, B11110111, B11101111, B11011111, B01111111};
    byte GroundLEDs [17] = {B00001000, B00000100, B00000010, B00000001, B00000001, B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B10000000, B10000000, B01000000, B00010000};
    for(int i=0;i<sizeof(ledStates);i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, ledStates[i]);
      shiftOut(dataPin, clockPin, GroundLEDs[i]);  
      digitalWrite(latchPin, HIGH);
      delay(count);
      reset();
    }
    count = count - 1;
  }
}


void srdce() {
  byte ledStates[8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111101};
  byte GroundLEDs [8] = {B00011000, B00111100, B01111110, B11111111, B11111111, B11111111, B01100110, B01100110};
  for(int i=0;i<sizeof(ledStates);i++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, ledStates[i]);
    shiftOut(dataPin, clockPin, GroundLEDs[i]);  
    digitalWrite(latchPin, HIGH);
  }
}


// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
     


