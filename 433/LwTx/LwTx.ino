#include <EEPROM.h>
#include <LwRx.h>
 
void setup() {
  // nastaví datový vývod přijmače na pin 2
  lwrx_setup(2);
  // zapne seriovou komunikaci
  Serial.begin(9600);
  }
 
void loop() {
  // proměná pro 10 hodnot
  byte data[10];
  // proměnná pro delku dat
  byte delka = 10;
 
  //když jsou přijata data
  Serial.println(lwrx_message());
  if (lwrx_message())
    {
    // ulož data do promené data
    lwrx_getmessage(data,delka);
   
    // opakuj pro i od 0 až do 9
    for (int i = 0; i<10; i++)
    {
      //vypíše data[pozice i]
      Serial.print(data[i]);
      Serial.print(" ");
    }
    // nový řádek
    Serial.println();
  }
}
