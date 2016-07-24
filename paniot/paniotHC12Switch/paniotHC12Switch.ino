void setup() {
 pinMode(7,OUTPUT); //  setPin on hc12
 Serial.begin(9600);
 while (!Serial) {}
 Serial.println("Serial is available");
 
 digitalWrite(7,LOW); // enter AT command mode
 Serial.print(F("AT\r\n")); // 9600, CH1, FU3, (F) to bypass flash memory
 delay(100);
 digitalWrite(7,HIGH); // enter transparent mode

}

void loop() {
  
}
