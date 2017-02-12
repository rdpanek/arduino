#include <ArduinoOTA.h>

void initOTA() {
  // Start OTA server.
  char* _deviceName = &deviceName[0];
  ArduinoOTA.setHostname(_deviceName);
  ArduinoOTA.onStart([]() {
    SPIFFS.end();
    Serial.println("Start updating ");
    ledBlick(1,10,"green");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    ledBlick(2,10,"green");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    ledBlick(1,10,"green");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    ledBlick(2,10,"red");
  });
  ArduinoOTA.begin();
}
