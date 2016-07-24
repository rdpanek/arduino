#include "U8glib.h" 
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET 



// encoder
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int buzzPin = 13;
boolean buzzPushed = false;
int encoderSwitchPin = 8; //push button switch

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;
int maxEncoded = 0;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  pinMode(encoderSwitchPin, INPUT);
  pinMode(buzzPin, OUTPUT);


  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on


  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

}

#define MENU_ITEMS 4
const char *menu_strings[MENU_ITEMS] = { "<- zpet", "Zapisovat na SD", "Tail SD", "Smazat SD" };

void drawMenu(void) {
  maxEncoded = MENU_ITEMS;
  uint8_t i, _i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x12);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == encoderValue ) {
      u8g.drawBox(0, i*h, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
}

void draw()
{
  u8g.setFont(u8g_font_profont12);   
  u8g.setPrintPos(5, 15);
  u8g.print("Hodnota: ");
  u8g.print(encoderValue);

 

  u8g.drawFrame(0,0,128,64);   
}  

void loop(){ 
  u8g.firstPage();  
  do {
    drawMenu();
  } while( u8g.nextPage() );

  
  //Do stuff here
  if(digitalRead(encoderSwitchPin)){
    buzzPushed = true;
  }else{
    if (buzzPushed == true)
      buzz();
      buzzPushed = false;
  }
  updateEncoder();
  //delay(1000); //just here to slow down the output, and show it will work  even during a delay
}

void buzz() {
  digitalWrite(buzzPin,HIGH);
  delay(20);
  digitalWrite(buzzPin,LOW);
}


void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  if(sum == 0b1101) encoderValue++;
  if(sum == 0b1110) encoderValue--;

  // osetreni, aby se hodnota nedostala pod nulu
  if (encoderValue < 0) encoderValue = 0;
  if (encoderValue > (maxEncoded - 1)) encoderValue = (maxEncoded - 1);
  lastEncoded = encoded; //store this value for next time
}
