#include <ArduinoOTA.h>

void initOTA() {
  // Start OTA server.
  char* _deviceName = &deviceName[0];
  ArduinoOTA.setHostname(_deviceName);
  ArduinoOTA.onStart([]() {
    SPIFFS.end();
    ledBlick(1,10);
    Serial.println("Start updating ");
  });
  ArduinoOTA.onEnd([]() {
    ledBlick(2,10);
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    ledBlick(1,10);
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    ledBlick(3,10);
  });
  ArduinoOTA.begin();
  ledBlick(1,10);  
}
