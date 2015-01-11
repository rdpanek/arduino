#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
#define LED 13
#define GSMReset 5

String GSMsignal;
char rx_buf[64];
char tx_buf[64];
byte rx_index = 0;
byte tx_index = 0;

void setup()
{
  Serial.begin(19200);
  SIM900.begin(19200);
  delay(1000);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  SIM900Init();
  memset(tx_buf, 0, sizeof(tx_buf));
  memset(rx_buf, 0, sizeof(rx_buf));
}

void SIM900Init()
{

  while (Serial.available() > 0) Serial.read();
  Serial.println F("****************************************");
  while (sendATcommand("AT","OK",2000) == 0);
  Serial.println F("SIM900 modul dostupny!");
  //otestuj registraci do site
  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
  sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  while (Serial.available() > 0) Serial.read();
  Serial.println F("SIM900 registrovan do site!");
  //nastaveni parametru pro obdrzeni casu ze site
  while( sendATcommand("AT+CLTS=1", "OK", 500) == 0 ); // get local timestamp
  while( sendATcommand("AT+CENG=3", "OK", 500) == 0 ); // engineer mode
  while (Serial.available() > 0) Serial.read();
  Serial.print("Casova znacka:");
  Serial.println(timeStamp());

  /*
   * detekce kvality signalu
   * - melo by se kontinualne zvysovat a pri problemu se signalem by mel zaznit akusticky signal
   * 2...30 -100dBm...-54dBm
   * 31 -52dBm nebo lepsi
   * 99 neni znamo, nebo problem s detekci
   */
  GSMsignal = sendATcommandResponse("AT+CSQ", "+CSQ:", 1000, 3);
  Serial.println("GSM signal quality:"+GSMsignal);
  if (GSMsignal.length()==3) GSMsignal = GSMsignal.substring(1);

  /*
   * Nazev operatora
   */
  String providerName = sendATcommandResponse("AT+COPS?","+COPS:",1000,20);
  uint8_t tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(tempIndex+1);
  tempIndex = providerName.indexOf('"');
  providerName = providerName.substring(0,tempIndex);
  Serial.println("GSM provider:"+providerName);
  
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
  Serial.println F("Zakladni parametry nastaveny!");
}


int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{

  //WDT_Restart(WDT);
  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  //clrSIMbuffer();
  SIM900.println(ATcommand);    // Send the AT command
  //WDT_Restart(WDT);

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do
  {
    //WDT_Restart(WDT);
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

  //clrSIMbuffer();
  return answer;
}


//AT prikaz s ocekavanou odpovedi a zbytkem
String sendATcommandResponse(char* ATcommand, char* expected_answer, unsigned int timeout, unsigned int buf)
{
  //WDT_Restart(WDT);
  uint8_t x = 0,  answer = 0;
  char response[150];
  unsigned long previous;
  String rest;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  //clrSIMbuffer();
  SIM900.println(ATcommand);    // Send the AT command


  x = 0;
  previous = millis();
  //WDT_Restart(WDT);
  // this loop waits for the answer
  do
  {
    //WDT_Restart(WDT);
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

  //p?e?ti zbytek - max 20 byte
  memset(response, '\0', buf);    // Initialize the string
  delay(100);
  for (x = 0; x < buf; x++) response[x] = SIM900.read();
  //clrSIMbuffer();
  for (x = 0; x < buf; x++) rest += char(response[x]);
  delay(100);
  return rest;

}


void checkSMS()
{
  
  char g;
  String gcmd;
  String number;
  uint8_t gindex;
  //clrSIMbuffer();
  SIM900.println("AT+CMGR=1");
  delay(100);
  
  while(SIM900.available()>0)
  {
    g=SIM900.read();
    gcmd += g;
    Serial.println(gcmd);
  }
}


// some struct
struct st_SMS {
   int index = 1;
  char status[16];
  char phonenumber[16]; 
  char datetime[25];
  char msg[161];
};

st_SMS sms;

int getSMS(int index) 
{
  // init args
  sms.index = index;
  memset(sms.status, 0, 16);
  memset(sms.phonenumber, 0, 16);
  memset(sms.datetime, 0, 25);
  memset(sms.msg, 0, 128);
  // flush serial buffer
  SIM900.flush();
  // AT+CMGR=1\r
  SIM900.print(F("AT+CMGR="));
  SIM900.print(sms.index);
  SIM900.print(F("\r"));
  // parse result
  // +CMGR: "REC READ","+33123456789","","2013/06/29 11:23:35+08"
  if (! SIM900.findUntil("+CMGR:", "OK")) 
    return 0; 
  Serial.println(sms.msg);
  SIM900.find("\"");
  SIM900.readBytesUntil('"', sms.status, sizeof(sms.status)-1);
  SIM900.find(",\"");
  SIM900.readBytesUntil('"', sms.phonenumber, sizeof(sms.phonenumber)-1);  
  SIM900.find(",\""); 
  SIM900.find("\",\"");
  SIM900.readBytesUntil('"', sms.datetime, sizeof(sms.datetime)-1);
  SIM900.find("\r\n");
  SIM900.readBytesUntil('\r', sms.msg, sizeof(sms.msg)-1);
  return 1;  
}


//Send SMS
void sendSMS(String number, String sms)
{
  Serial.println("SMS pro:"+number);
  Serial.println("Obsah:"+sms);
  delay(500);
  clrSIMbuffer();
  //number = "+420"+number;
  SIM900.println ("AT+CMGS=\""+number+"\"");
  delay(200);
  //toSerial();
  SIM900.println (sms);        // message to send
  delay(100);
  SIM900.write ((char) 26); //CTRL+Z
  delay(100);
  SIM900.println();
  delay(100);
  sendATcommand("AT+CMGD=1", "OK", 2000);
  sendATcommand("AT+CMGD=1,4", "OK", 2000);
  delay(500);
  clrSIMbuffer();
  Serial.println("SMS odeslana!");
}


void SMSparser(String command)
{
  if (command == "LED:ON")
  {
    digitalWrite(LED,HIGH);
    return;
  }
  if (command == "LED:OFF")
  {
    digitalWrite(LED,LOW);
    return;
  }
  Serial.println("Neznamy prikaz!");

}


String timeStamp()
{
  String ts="";
  ts = sendATcommandResponse("AT+CCLK?", "+CCLK: \"", 1000, 20);
  //13/11/04,15:23:19+04
  //return ("20"+ts.substring(0,2)+ts.substring(3,5)+ts.substring(6,8)+ts.substring(9,11)+ts.substring(12,14)+ts.substring(15,17));
  return ts;
}


void clrSIMbuffer()
{
  while(SIM900.available()>0)
  {
    delay(1);
    SIM900.read();
  }
}

void loop()
{
  delay(1000);
  while(SIM900.available()) {
    // check incoming char
    int c = SIM900.read();
    if (c != 0)
      rx_buf[rx_index] = c;
    if (c == '\n') {
      // search notice message
      // +CMTI: "SM" for incoming SMS notice : launch sms job
      Serial.println(rx_buf);
      if (strncmp(rx_buf, "+CMTI: \"SM\"", 11) == 0) {
        if (getSMS(sms.index)) {
          printf_P(PSTR("%s\n"), sms.index);
          printf_P(PSTR("%s\n"), sms.status);
          printf_P(PSTR("%s\n"), sms.phonenumber);
          printf_P(PSTR("%s\n"), sms.datetime);
          printf_P(PSTR("%s\n"), sms.msg);
        }
      } 
    }
  }
}
 
