#include <ESP8266WiFi.h>          // Knihovna pro praci s WIFI
#include <DNSServer.h>            // Local DNS Server presmeruje vsechny requesty do configuration portal
#include <WiFiManager.h>          // Knihovna pro praci s WIFI


String nazevZarizeni = "Muj-teplotni-senzor";


WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
}

int configPortalTimeout = 120;

void setup() {
  Serial.begin(115200);

  // vymaze nastaveni z EEPROM - pri beznem pouzivani, nechat zakomentovane
  // wifiManager.resetSettings();

  // bude vypisovat do monitoru to co dela
  wifiManager.setDebugOutput(true);

  // Nastaveni
  wifiManager.setAPCallback(configModeCallback);
  
  // upravi html nastavovaciho formulare
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");

  // Limit, po ktery se musi zarizeni pripojit do wifi, nebo telefon k zarizeni - po prekroceni dojde k restartu
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  
  // Udalost, pri ulozeni nastaveni wifi
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // nazev zarizeni
  char* _nazevZarizeni = &nazevZarizeni[0];
  wifi_station_set_hostname(_nazevZarizeni);
  
  // pokud se nezdari pripojit ve stanoveny limit - zarizeni bude restartovano
  if(!wifiManager.autoConnect(_nazevZarizeni)) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  Serial.println("-- start --");
}

void loop() {

} 

