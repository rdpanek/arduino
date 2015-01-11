#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

void setup() 
{
  Serial.begin(115200);
  SIM900.begin(9600);           //Open Serial connection at baud 2400
  delay(2000);
  SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
  delay(1500);
  Serial.print(SIM900.available());
  SIM900.println("AT+CMGS=\"+420731011200\"\r");   //phone number to which you want to send sms
  delay(1000);
  SIM900.println("Tajna zprava");           //SMS body 
  delay(1000); 
  SIM900.write(0x1A);           // sends ctrl+z end of message 
}

void loop() 
{
  
}
