#include <ArduinoJson.h>
#include <FS.h>
String configuration = "";

// use
// -> setup fsInit();
//saveConfig();
//loadConfig();

void fsInit() {
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }  
}

void praseJsonToString(JsonObject& json) {
  configuration = "{";
  for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
  {
   configuration += "\""+String(it->key)+"\":";
   configuration += "\""+String(it->value.asString())+"\","; 
  }
  configuration += "}";  
}

void saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["deviceName"] = deviceName.c_str();
  json["deviceLocation"] = deviceLocation.c_str();
  json["targetDeviceIp"] = targetDeviceIp.c_str();
  json["elasticsearchUri"] = elasticsearchUri.c_str();
  File configFile = SPIFFS.open("/config.json", "w+");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
  }
  
  json.printTo(configFile);
  praseJsonToString(json);
  Serial.println("Save config success.");
}


void loadConfig() {
  if (!SPIFFS.exists("/config.json")) saveConfig();
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
  }

  deviceName = json["deviceName"];
  Serial.print("deviceName: ");
  Serial.println(deviceName);

  deviceLocation = json["deviceLocation"];
  Serial.print("deviceLocation: ");
  Serial.println(deviceLocation);

  targetDeviceIp = json["targetDeviceIp"];
  Serial.print("targetDeviceIp: ");
  Serial.println(targetDeviceIp);

  elasticsearchUri = json["elasticsearchUri"];
  Serial.print("elasticsearchUri: ");
  Serial.println(elasticsearchUri);
  
  praseJsonToString(json);
  Serial.println("Load config success.");
}

