// importujeme knihovnu
#include <EEPROM.h>

long cislo = 0;

void setup()
{
  //postupně zapíše hodnotu na adresy 0 až 5
  //EEPROM.write(0,35);
  //EEPROM.write(1,47);
  //EEPROM.write(2,85);
  //EEPROM.write(3,96);
  //EEPROM.write(4,17);

  Serial.begin(9600);
  cislo = EEPROM.read(0);
  Serial.println(cislo);
  cislo = cislo * 1000;
  Serial.println(cislo);
}

void loop(){}
