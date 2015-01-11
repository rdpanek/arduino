#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
 
int LED = 13;

void setup()
{
pinMode(LED, OUTPUT);
Serial.begin(19200);
mySerial.begin(19200);  // the GPRS baud rate
}
 
void loop()
{
  delay(300);
  if (Serial.available())
  {
    String content = "";
    char character;
    while(Serial.available()) {
      character = Serial.read();
      content += character;
    }
    content.trim();
    if (content.equals("led")) {
      digitalWrite(LED, HIGH);
      delay(3000);
      digitalWrite(LED, LOW);

    } else if (content == "call") {
      mySerial.println("ATD + +420731011200;"); //Calling to my cell. I just deleted part of my phone number cus i dont want to shove it
      delay(100);
      mySerial.println();
      delay(7000);       
      mySerial.println("ATH");
 
    } else if (content == "newSms") {
      mySerial.println("AT+CNMI=?\r");  //asking for new sms message indication
      delay(100);

    } else if (content == "readFirstMessage") {
      mySerial.println("AT+CMGR=1\r");  //read first message
      delay(100);

    } else if (content == "storeMessaging") {
      mySerial.println("AT+CPMS=?\r");  //store messaging
      delay(100);

    } else if (content == "deleteFirstMessage") {
      mySerial.println("AT+CMGD=1\r");  //delete first message
      delay(100);
 
    } else if (content == "qualitySignal") {
      mySerial.println("AT+CSQ=?\r");  //asking for signal quality report
      delay(100);

    } else if (content == "carrier") {
      mySerial.println("AT+CREG?\r");  //asking for carrier
      delay(100);

    } else if (content == "simStatus") {
      mySerial.println("AT+CPIN?\r");  //sim card is found
      delay(100);
    }
    
  }
  if (mySerial.available())
  {
     Serial.write(mySerial.read());
  }
}
 
