/*
Arduino Real Time Oled Clock
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

RTC_DS3231 rtc;

char rtcDay[20];
char rtcMonth[20];
char rtcYear[20];
char rtcHour[20];
char rtcMinute[20];
char rtcSecond[20];

void setup()   
{                
  Serial.begin(9600);
  //delay(3000);  // wait 3 seconds for serial monitor opening

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C

  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // time can be adjusted manually like
    // July 22, 2018 at 8am you would call:
    // rtc.adjust(DateTime(2018, 7, 22, 8, 0, 0));
  }
}


void loop() 
{
  DateTime now = rtc.now();
  display.clearDisplay();
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2,1);

  // TIME
  uint8_t intHour = now.hour();
  uint8_t intMinute = now.minute();
  uint8_t intSecond = now.second();
  snprintf (rtcHour, 20, "%.2i", intHour);
  snprintf (rtcMinute, 20, "%.2i", intMinute);
  snprintf (rtcSecond, 20, "%.2i", intSecond);
  
  //DATE
  uint8_t intDay = now.day();
  uint8_t intMonth = now.month();
  uint16_t intYear = now.year();
  snprintf (rtcDay, 20, "%.2i", intDay);
  snprintf (rtcMonth, 20, "%.2i", intMonth);
  snprintf (rtcYear, 20, "%.4i", intYear);
  
  display.print(rtcHour); // hour
  display.print(":");
  display.print(rtcMinute); // minute
  display.print(":");
  display.print(rtcSecond); // second
  display.print(" ");
  display.print(rtcDay); // day
  display.print("-");
  display.print(rtcMonth); // month
  display.print("-");
  display.println(rtcYear); // year

  display.drawLine(0,16, display.width()-1, 16, WHITE);

  display.setCursor(2,18);
  display.println("Arduino RTC OLED");

  display.display();
  delay(1000);  // 1 second delay
}

