#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

#include <RemoteReceiver.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int counter = 0, identicalCode = 3, buzzDelay = 100, receiveCode = 0, period = 0;
int BUZZ = 3;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, HIGH);
  delay(buzzDelay);
  digitalWrite(BUZZ, LOW);
  
  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 3 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  RemoteReceiver::init(0, identicalCode, showCode);
  lcd.init();// inicializuje displej
   
  lcd.backlight(); // zapne podsvětlení
  lcd.clear();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print(counter);
  lcd.print("# ");
  lcd.setCursor(0,1);
  lcd.print(receiveCode);
  lcd.print(" ");
  lcd.print(period);
}

// Callback function is called only when a valid code is received.
void showCode(unsigned long _receivedCode, unsigned int _period) {
  // Note: interrupts are disabled. You can re-enable them if needed.
  receiveCode = _receivedCode;
  period = _period;
  counter = ++counter;
  Serial.println(receiveCode);
  digitalWrite(BUZZ, HIGH);
  delay(buzzDelay);
  digitalWrite(BUZZ, LOW);
}


