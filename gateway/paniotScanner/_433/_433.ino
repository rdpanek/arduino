#include <LwTx.h>

// tyto čísla budeme odesílat
byte data[] = {1,2,3,4,5,6,7,8,9,0};

void setup() {
  // Zapne ser. komunikaci 
  Serial.begin(9600);
  
  // Data do vysílače jsou na pin 3.
  // Další číslo je počet opakovaní vysílaní
  // za sebou, v našem připadě 2x
  lwtx_setup(3, 2);
}

void loop()
{
  // když je vysílač nezaneprázdněn
  if (lwtx_free())
   {
   //odešli data
   lwtx_send(data);
   // odešle zprávu na serial o odeslání
   Serial.println("Data odeslana"); 
   //čekej 5 sekund
   delay(5000);
   }
}
