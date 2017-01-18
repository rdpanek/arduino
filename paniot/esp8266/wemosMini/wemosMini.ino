#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>

String ntpTime = "00:00:00";
String ntpDate = "1970-01-01";
String nazevZarizeni = "Muj-teplotni-senzor";

void processSyncEvent(NTPSyncEvent_t ntpEvent) {
  if (ntpEvent) {
    if (ntpEvent == noResponse)
      Serial.println("NTP server neni dostupny");
    else if (ntpEvent == invalidAddress)
      Serial.println("Invalidni adresat NTP serveru");
  }
  else {
    Serial.print("NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


WiFiManager wifiManager;
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback() {
  Serial.println("Save wifi configuration.");
}

int configPortalTimeout = 120;


void setup() {
  Serial.begin(115200);

  wifiManager.setDebugOutput(true);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wifiManager.setConfigPortalTimeout(configPortalTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  char* _nazevZarizeni = &nazevZarizeni[0];
  wifi_station_set_hostname(_nazevZarizeni);
  if(!wifiManager.autoConnect(_nazevZarizeni)) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  // ntp
  if(WiFi.status() == WL_CONNECTED) {
    NTP.begin("pool.ntp.org", 1, true);
    NTP.setInterval(63);
  }

  NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
  });  

  Serial.println("-- start --");
}

void loop() {
  static int last = 0;

  if (syncEventTriggered) {
    processSyncEvent(ntpEvent);
    syncEventTriggered = false;
  }

  if ((millis() - last) > 5100) {
    last = millis();
    ntpTime = NTP.getTimeStr();
    String _date = NTP.getDateStr();
    _date.replace("/", "-");
    String _month,_day;
    if (month() < 10 ) {_month = "0"+String(month());} else {_month = String(month());}
    if (day() < 10 ) {_day = "0"+String(day());} else {_day = String(day());}
    ntpDate = String(year()) +"-"+ _month +"-"+ _day;

    Serial.println("NTP Time: " + ntpTime);
    Serial.println("NTP Date: " + ntpDate);
  }  
} 

