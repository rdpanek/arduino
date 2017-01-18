#include <ArduinoJson.h> // documentation https://github.com/bblanchon/ArduinoJson/wiki/Quick%20Start
#include <FS.h> // documentation https://github.com/esp8266/Arduino/blob/master/doc/filesystem.md

String konfiguracniSoubor = "/konfigurace.json";

String nazevZarizeni = "Teplomer";
int pocetTeplomeru = 1;
float namerenaTeplota = 25.00;

String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

void saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["nazevZarizeni"] = nazevZarizeni.c_str();
  json["pocetTeplomeru"] = pocetTeplomeru;
  json["namerenaTeplota"] = namerenaTeplota;
  File configFile = SPIFFS.open(konfiguracniSoubor, "w+");
  if (!configFile) {
    Serial.println("Nejde otevrit soubor pro ulozeni konfigurace.");
  }
  
  json.printTo(configFile);
  Serial.println("Ulozeni bylo uspesne.");
}


void loadConfig() {
  if (!SPIFFS.exists(konfiguracniSoubor)) saveConfig();
  File configFile = SPIFFS.open(konfiguracniSoubor, "r");
  if (!configFile) {
    Serial.println("Nelze otevrit konfiguracni soubor");
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Konfiguracni soubor je prilis velky");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Chyba v parsovani konfiguracniho souboru");
  }

  nazevZarizeni = json["nazevZarizeni"];
  Serial.print("nazevZarizeni: ");
  Serial.println(nazevZarizeni);

  pocetTeplomeru = json["pocetTeplomeru"];
  Serial.print("pocetTeplomeru: ");
  Serial.println(pocetTeplomeru);

  namerenaTeplota = json["namerenaTeplota"];
  Serial.print("namerenaTeplota: ");
  Serial.println(namerenaTeplota);
  
  Serial.println("Nacteni konfigurace bylo uspesne.");
}

void setup() {
  Serial.begin(115200);

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

  loadConfig();


  Serial.println("-- start --");
}

void loop() {

} 

