#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xFFFF
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTGREY 0xC618

#define color = GREEN;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  tft.fillScreen(BLACK);
}

void loop(void) {
  //tft.drawRect(5, 5, 230, 310, WHITE);
  //tft.fillRect(6, 6, 228, 308, LIGHTGREY);
  tft.setRotation(3);
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Kontrola cidel:");

  tft.setCursor(10, 30);
  tft.setTextColor(YELLOW);  tft.setTextSize(1);
  tft.print("Pracovna:22.0:7.8V:48%");

  tft.setCursor(10, 50);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("Pocet: ");
  tft.setTextColor(BLUE);
  tft.print("0");

  tft.setCursor(10, 70);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("Mod modulu: ");
  tft.setTextColor(BLUE);
  tft.print("Receive Mode");
}
