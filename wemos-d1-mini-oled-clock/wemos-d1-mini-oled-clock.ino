/*
WeMos D1 Mini Real Time Oled Clock
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include <Wire.h>
#include "SSD1306Wire.h"
#include <RTClib.h>

// Initialize the OLED display using Wire library
// display(0x3c, SDA, SCL)
SSD1306Wire  display(0x3c, D2, D1);

RTC_DS3231 rtc;

#ifndef ESP8266
 while (!Serial);
#endif

char rtcDay[20];
char rtcMonth[20];
char rtcYear[20];
char rtcHour[20];
char rtcMinute[20];
char rtcSecond[20];

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  Serial.println("Time setting!");
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // time can be adjusted manually like
  // July 22, 2018 at 8am you would call:
  // rtc.adjust(DateTime(2018, 7, 22, 8, 0, 0));
}

void loop() 
{
  DateTime now = rtc.now();
  
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

  String timeString = String(rtcHour) + ":" + String(rtcMinute) + ":" + String(rtcSecond);
  String dateString = String(rtcDay) + "-" + String(rtcMonth) + "-" + String(rtcYear);
  String rtcString = timeString + " " + dateString;

  Serial.println(rtcString);
  
  display.clear();  // clear the display

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, rtcString);
  display.drawHorizontalLine(0, 20, display.getStringWidth(rtcString));
  
  display.display();

  delay(1000); // 1 second delay
}
