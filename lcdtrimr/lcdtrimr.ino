#include <LiquidCrystal.h>

LiquidCrystal MojeLCD1(2,3,4,5,6,7);

const int analogPin = A0;
int hodnota = 0;


void setup() { 
  MojeLCD1.begin(20, 4);
  MojeLCD1.print("Cteni hodnoty z A0");
}

void loop() {

  hodnota = analogRead(analogPin);
  MojeLCD1.setCursor(0,3);
  MojeLCD1.print("hodnota = ");
  MojeLCD1.print(hodnota);
  MojeLCD1.print(" ");
  
  delay(100);
  
}

