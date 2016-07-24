#include <SD.h>
#include <SdFat.h>
#include "U8glib.h" 
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RTCx.h>


SoftwareSerial SIM900(10, 9);
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET  

String qualitySignal;

boolean firstStart = true;
String carrierName = "PanIoT GSM";
String dateTime;
String _date;

// SD
const int chipSelect = 4; 
Sd2Card card;
SdVolume volume;
File dataFile;
int dataFileSize = 0;
String sdInfo;

String ram;

// ISO 8601 1994-11-05T13:15:30Z
void getDateTime(struct RTCx::tm *tm)
{
  dateTime = tm->tm_year + 1900;
  dateTime += "-";
  dateTime += tm->tm_mon + 1;
  dateTime += "-";
  dateTime += tm->tm_mday;
  dateTime += "T";
  dateTime += tm->tm_hour;
  dateTime += ":";
  dateTime += tm->tm_min;
  dateTime += ":";
  dateTime += tm->tm_sec;

  _date = tm->tm_year + 1900;
  _date += "-";
  _date += tm->tm_mon + 1;
  _date += "-";
  _date += tm->tm_mday;
}

void updateSignalStrength(void)
{
  qualitySignal = sendATcommandResponse("AT+CSQ", "+CSQ:", 2000, 3);
  qualitySignal.trim();
}

void getCarrierName(void)
{
  String providerName = sendATcommandResponse("AT+COPS?","+COPS:",1000,20);
  uint8_t tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(tempIndex+1);
  tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(0,tempIndex);
  carrierName = providerName.substring(0,17);  
}

void SIM900Init()
{
  firstStart = false;
  while (Serial.available() > 0) Serial.read();

  // je SIM900 modul dostupny?
  while (sendATcommand("AT","OK",2000) == 0);
  
  //otestuj registraci do site
  
  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
  sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  while (Serial.available() > 0) Serial.read();
  
  updateSignalStrength();
  getCarrierName();
}

void homeDisplay()
{
  u8g.setFont(u8g_font_profont12);   
  u8g.setPrintPos(5, 12);
  u8g.print(carrierName);

  u8g.setPrintPos(5, 25);
  u8g.print("T:");
  u8g.print(dateTime);

  u8g.setPrintPos(5, 37);
  u8g.print("Signal:");
  u8g.print(qualitySignal);

  u8g.setPrintPos(5, 49);
  u8g.print("SD:");
  u8g.print(sdInfo);

  u8g.setPrintPos(5, 61);
  u8g.print("RAM:");
  u8g.print(ram);
  
  u8g.drawFrame(0,0,128,64);   
}  


void getCardInfo() {
  if (card.init(SPI_HALF_SPEED, chipSelect)) {
    switch(card.type()) {
      case SD_CARD_TYPE_SD1:
        sdInfo = "SD1";
        break;
      case SD_CARD_TYPE_SD2:
        sdInfo = "SD2";
        break;
      case SD_CARD_TYPE_SDHC:
        sdInfo = "SDHC";
        break;
      default:
        sdInfo = "???";
    }
    sdInfo += " ";
    sdInfo += dataFileSize;
    sdInfo += " kB";
  } else {
    sdInfo = "je vlozena karta?";
  }
  return;
}

void writeToFile() {
  dataFile = SD.open("GSM.txt", FILE_WRITE);
  if (!dataFile) {
      Serial.println("nelze otevrit soubor");
  } else {
    String dataString = "\"";
    dataString += dateTime;
    dataString += "\",\"";
    dataString += qualitySignal;
    dataString += "\",\"";
    dataString += carrierName;
    dataString += "\",\"";
    dataString += ram;
    dataString += "\"";
    Serial.println(dataString);
    dataFile.println(dataString);
    dataFileSize = (dataFile.size() / 1000);
    dataFile.close();
  }
}

void setup()
{
  Serial.begin(19200);
  SIM900.begin(19200);
  
  Wire.begin();
  // The address used by the DS1307 is also used by other devices (eg
  // MCP3424 ADC). Test for a MCP7941x device first.
  uint8_t addressList[] = {RTCx::MCP7941xAddress,
         RTCx::DS1307Address};
  
  // Autoprobe to find a real-time clock.
  if (rtc.autoprobe(addressList, sizeof(addressList))) {
    // Found something, hopefully a clock.
    dateTime = "DS1307 found";
    switch (rtc.getDevice()) {
      case RTCx::DS1307:
      break;
    }
  }
  else {
    dateTime = "No RTCx found";
    return;
  }
  
  // Enable the battery backup. This happens by default on the DS1307
  // but needs to be enabled on the MCP7941x.
  rtc.enableBatteryBackup();
  
  // Ensure the oscillator is running.
  rtc.startClock();
  rtc.setSQW(RTCx::freq4096Hz);
  
  delay(1000);
  
  //SD
  pinMode(SS, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Je vlozena karta?");
  }
}
 
void loop()
{
  // inicialize only with first loop
  if (firstStart == true) SIM900Init(); else updateSignalStrength();
  
  // check live SIM, if is die = reset
  if (sendATcommand("AT","OK",2000) == 0)
  {
    firstStart = true;
    carrierName = "SIM nekomunikuje";
    qualitySignal = "00";
  }
  
  // RTC
  struct RTCx::tm tm;
  rtc.readClock(tm);
  getDateTime(&tm);
  ram = freeRam();

  u8g.firstPage();  
  do {
    homeDisplay();
  } while( u8g.nextPage() );

  getCardInfo();

  writeToFile();
}


int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  SIM900.println(ATcommand);    // Send the AT command

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do
  {
    if (SIM900.available() != 0) {
      // if there are data in the UART input buffer, reads it and checks for the asnwer
      response[x] = SIM900.read();
      x++;
      // check if the desired answer  is in the response of the module
      if (strstr(response, expected_answer) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}

String sendATcommandResponse(char* ATcommand, char* expected_answer, unsigned int timeout, unsigned int buf)
{
  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;
  String rest;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);
  SIM900.println(ATcommand);    // Send the AT command


  x = 0;
  previous = millis();
  do
  {

    if (SIM900.available() != 0) {
      // if there are data in the UART input buffer, reads it and checks for the asnwer
      response[x] = SIM900.read();
      x++;
      // check if the desired answer  is in the response of the module
      if (strstr(response, expected_answer) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while ((answer == 0) && ((millis() - previous) < timeout));

  memset(response, '\0', buf);    // Initialize the string
  delay(100);
  for (x = 0; x < buf; x++) response[x] = SIM900.read();
  for (x = 0; x < buf; x++) rest += char(response[x]);
  delay(100);
  return rest;

}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}
