// pin p?ipojen na RCLK registru (12)
int latchPin = 9;
//Pin connected to SRCLK registru (11)
int clockPin = 10;
////Pin p?ipojen na Data pin registu (14)
int dataPin = 8;

 
// data ulo?en� v poli
// v bin�rn� soustav? pro v?t?�
// p?ehlednost
byte data[] =
  {
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000
  };
 
 
void setup() {
  // nastav� na?e piny pro regist na v�stupn�
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
 
void loop() {
  // funkce projde v?echny byte v poli data
  for (int index = 0; index < sizeof(data); index++) {
    // kdy? d�me latchPin na LOW mu?eme
    // do registru poslat data
    digitalWrite(latchPin, LOW);
    // a to pomoc� funkce shiftOut, kter�
    shiftOut(dataPin, clockPin, MSBFIRST, data[index]);  
 
    // jakmile d�le latchPin na HIGH,
    // data se objev� na v�stupu
    digitalWrite(latchPin, HIGH);
   
    // po?k� 50ms, aby jsme v�sledek vid?li
    delay(30);
  }
 
  }