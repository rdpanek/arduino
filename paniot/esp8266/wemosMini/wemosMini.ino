#include <OneWire.h>
#include <DallasTemperature.h>

const int pinCidlaDS = D3;
float dallasTemperature = 0;


OneWire oneWireDS(pinCidlaDS);
DallasTemperature dallasDS(&oneWireDS);


void setup() {
  Serial.begin(115200);

  Serial.println("-- start --");
}

void loop() {
  dallasDS.requestTemperatures();
  dallasTemperature = dallasDS.getTempCByIndex(0);
  
  Serial.print("Teplota: ");
  Serial.print(dallasTemperature);
  Serial.println(" °C");

  delay(2000);
} 

