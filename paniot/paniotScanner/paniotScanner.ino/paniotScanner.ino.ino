#include <RemoteReceiver.h>

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
 
  RemoteReceiver::init(0, identicalCode, showCode);
}

void loop() {}

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


