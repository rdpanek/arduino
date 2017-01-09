#include <TimeLib.h>
#include <NtpClientLib.h>

String ntpTime = "00:00:00";
String ntpDate = "1970-01-01";



void processSyncEvent(NTPSyncEvent_t ntpEvent) {
  if (ntpEvent) {
    Serial.print("Time Sync error: ");
    if (ntpEvent == noResponse)
      Serial.println("NTP server not reachable");
    else if (ntpEvent == invalidAddress)
      Serial.println("Invalid NTP server address");
  }
  else {
    Serial.print("Got NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


void syncNtp() {
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
