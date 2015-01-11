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
  if (Serial.available())
  {
    char com = Serial.read();
    switch (com)
    {
      case '1':
      digitalWrite(LED, HIGH);
      delay(3000);
      digitalWrite(LED, LOW);
      break;

      case '2':
      mySerial.println("ATD + +420731011200;"); //Calling to my cell. I just deleted part of my phone number cus i dont want to shove it
      delay(3000);
      mySerial.println();
      delay(7000);       
      mySerial.println("ATH");
      break;
 
      case '3':
      mySerial.println("AT+CNMI=?");  //asking for new sms message indication
      delay(100);
      break;

      case '4':
      mySerial.println("AT+CMGR=1");
      delay(100);
      break;

      case '5':
      mySerial.println("AT+CPMS=?");  //store messaging
      delay(100);
      break;

      case '6':
      mySerial.println("AT+CMGD=1\r");  //delete first message
      delay(100);
      break;
 
      case '7':
      mySerial.println("AT+CSQ\r");  //asking for signal quality report
      delay(100);
      break;

      case '8':
      mySerial.println("AT+COPS?\r");  //asking for carrier
      delay(100);
      break;

      /*
      case '9':
      mySerial.println("AT+CPIN?\r");  //sim card is found
      delay(100);
      break;
      */
      case '9':
      mySerial.write("AT+CMGF=1\r");           //set GSM to text mode
      delay(1500);
      mySerial.println("AT+CMGS=\"+420731011200\"");   //phone number to which you want to send sms
      delay(1000);
      mySerial.println("Tajna zprava");           //SMS body 
      delay(1000); 
      mySerial.println("\r");           // sends ctrl+z end of message 
      delay(100);
      break;
    }  
  }
  if (mySerial.available())
  {
     Serial.write(mySerial.read());
  }
}
