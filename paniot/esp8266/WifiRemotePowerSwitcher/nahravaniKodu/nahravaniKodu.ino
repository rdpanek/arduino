/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    Serial.println(mySwitch.getReceivedValue());
    Serial.println(mySwitch.getReceivedBitlength());
    Serial.println(mySwitch.getReceivedDelay());
    Serial.println(mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
}
