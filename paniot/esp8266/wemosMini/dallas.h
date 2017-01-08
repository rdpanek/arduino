#include <OneWire.h>
#include <DallasTemperature.h>

const int pinCidlaDS = 13;

OneWire oneWireDS(pinCidlaDS);
DallasTemperature dallasDS(&oneWireDS);


// get temperature
//dallasDS.requestTemperatures();
//dallasDS.getTempCByIndex(0)
