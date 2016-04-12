#include "U8glib.h"
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

boolean stateOfArm = false;
boolean isStartGateway = true;
String carrierName = "Cekam na SIM";
String qualitySignal;
String GSMsignal;
String incomingCommand;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup(void) 
{
  Serial.begin(19200);
  SIM900.begin(19200);
  delay(1000);
}

void SIM900Init()
{
  isStartGateway = false;
  while (Serial.available() > 0) Serial.read();

  // je SIM900 modul dostupny?
  while (sendATcommand("AT","OK",2000) == 0);
  
  //otestuj registraci do site
  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
  sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  while (Serial.available() > 0) Serial.read();
  
  //nastaveni parametru pro obdrzeni casu ze site
  while( sendATcommand("AT+CLTS=1", "OK", 500) == 0 );
  while( sendATcommand("AT+CENG=3", "OK", 500) == 0 );

  /*
   * detekce kvality signalu
   * - melo by se kontinualne zvysovat a pri problemu se signalem by mel zaznit akusticky signal
   * 2...30 -100dBm...-54dBm
   * 31 -52dBm nebo lepsi
   * 99 neni znamo, nebo problem s detekci
   */
  qualitySignal = sendATcommandResponse("AT+CSQ", "+CSQ:", 1000, 3);
  qualitySignal.trim();

  /*
   * Nazev operatora
   */
  String providerName = sendATcommandResponse("AT+COPS?","+COPS:",1000,20);
  uint8_t tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(tempIndex+1);
  tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(0,tempIndex);
  carrierName = providerName.substring(0,17);
  
  /*
   * vyber sms formatu - text mod
   * defaultne je zvolen PDU mod
   */
  sendATcommand("AT+CMGF=1", "OK", 500);
  
  /*
   * zakaz indikace SMS
   * nechapu proc
   */
  sendATcommand("AT+CNMI=0,0", "OK",500);

  /*
   * povolene volani a CLIP notifikace
   * nechapu moc o co jde
   */
  sendATcommand("AT+CLIP=1", "OK", 500);
  
  /*
   * smaz vsechny SMSky
   * zvlastni, cekal bych DEL READ a DEL UNSET nebo vsechno DEL ALL protoze vyse
   * se nastavuje text mod, tyto volby jsou pro PDU mod, tedy:
   * delete all read messages a delete all unset SMS - pricemz mohl jednoduse pouzit
   * volbu 6 a tedy delete all SMS
   */
  sendATcommand("AT+CMGD=1,4", "OK", 2000);
  sendATcommand("AT+CMGD=1", "OK", 2000);
}


void updateSignalStrength(void)
{
  qualitySignal = sendATcommandResponse("AT+CSQ", "+CSQ:", 1000, 3);
  qualitySignal.trim();
}


void updateStateOfArm(void)
{
  if (stateOfArm == true)
  {
    stateOfArm = false;
    sendNotification("Zapezpeceni je vypnute.", "731011200");
  }
  else
  {
    stateOfArm = true;
    sendNotification("Zapezpeceni je aktivni.", "731011200");
  }
}

void draw(void) {
  
  // carrier section
  u8g.setFont(u8g_font_6x10);

  // carrier name
  u8g.setPrintPos(0, 13);
  u8g.print(carrierName);

  // quality of signal
  qualitySignal.toInt();
  u8g.setPrintPos(110, 13);
  u8g.print(qualitySignal);

  // notice of state
  if (stateOfArm && isStartGateway == false)
  {
    u8g.setColorIndex(1);
    u8g.drawBox(0, 40, 72, 20);
    u8g.setPrintPos(5,55);
    u8g.setFont(u8g_font_courB10);
    u8g.setColorIndex(0);
    u8g.print("AKTIVNI");
    u8g.setColorIndex(1);
  }
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
  char response[150];
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
  //clrSIMbuffer();
  for (x = 0; x < buf; x++) rest += char(response[x]);
  delay(100);
  return rest;

}

void sendNotification(String message, String cellNumber)
{
  String c = "AT+CMGS=\"+420";
  c += cellNumber;
  c += "\"";

  SIM900.println(c);
  delay(1000);
  SIM900.println(message); 
  delay(1000); 
  SIM900.write(0x1A);
  delay(2000);
}

void checkIncomingCommand(void)
{
  String incomingCommand = sendATcommandResponse("AT+CMGR=1","+CMGR:",1000,70);
  incomingCommand.replace("\n","#");
  if (incomingCommand.indexOf("REC UNREAD") >=0 && incomingCommand.indexOf("731011200") >=0)
  {
    sendATcommand("AT+CMGD=1,4", "OK", 2000);
    sendATcommand("AT+CMGD=1", "OK", 2000);
    updateStateOfArm();
  }
  incomingCommand = "";
}

void loop(void) {
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  // inicialize only with first loop
  if (isStartGateway == true) SIM900Init(); else updateSignalStrength();

  // check live SIM, if is die = reset
  if (sendATcommand("AT","OK",2000) == 0)
  {
    isStartGateway = true;
    carrierName = "SIM nekomunikuje";
    qualitySignal = "";
  } else {
    checkIncomingCommand();
  }
  delay(500);
}
