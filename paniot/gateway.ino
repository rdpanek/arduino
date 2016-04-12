#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
int LED = 13;
String command;
boolean stateOfGateway = false;

void setup()
{
pinMode(LED, OUTPUT);
Serial.begin(19200);
mySerial.begin(19200);  // the GPRS baud rate
}
 
void loop()
{
  updateStateOfGateway();
}

void updateStateOfGateway()
{
  mySerial.println("AT+CMGR=1\r");
  delay(100);
  while(mySerial.available() >0)
  {
    char c = mySerial.read();
    command += c;
    if (c == '\n')
    {
      if (command.indexOf("731011200") >=0)
      {
        if (stateOfGateway == false)
        {
          stateOfGateway = true;
          digitalWrite(LED, HIGH);
        } else if (stateOfGateway == true)
        {
          stateOfGateway = false;
          digitalWrite(LED, LOW); 
        }
        mySerial.println("AT+CMGD=1\r");
        delay(3000);
      }
      command = "";
      break;
    }
  }  
}
