/*
Home Air Quality Station
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar

Project allows to measure basic air parameters such as temperature, humidity, pressure
and particulate matter concentration (1, 2.5 and 10 um).
Monitoring station was built using
  DOIT ESP32 DevKit v1
  Plantower PMS7003 particulate matter sensor
  BME280 tempetarure, humidity and pressure sensor
  SSD1306 OLED Screen (optional)
  DS3231 Real Time Clock (optional)

Following libraries was used for this project
  https://github.com/Takatsuki0204/BME280-I2C-ESP32
  https://github.com/fu-hsi/PMS
  https://github.com/knolleary/pubsubclient
  https://github.com/ThingPulse/esp8266-oled-ssd1306 (optional)
  https://github.com/adafruit/RTClib (optional)

DOIT ESP32 DEVKIT V1 PIN DEFINITION:
  SDA - 21
  SCL - 22

Notes:
Readings can be vieved using 2 available ESP32 serial monitors.
To avoid problems with pin definition, Serial1 pins
have been changed from 9, 10 to 4, 2 as follows:
  #define RX1 4
  #define TX1 2
Sketch may not compile without this change.
Serial2 does not require redefinition.
Further explanation can be found on Andreas Spiess account:
https://www.youtube.com/watch?v=GwShqW39jlE

To make sure I2C addresses were parsed correctly you can use
I2C scanner sketch available on github
  https://gist.github.com/tfeldmann/5411375
By default sensors uses following i2c addresses:
  BME280  - 0x76
  SSD1306 - 0x3c
  RTC DS3231  - 0x68
*/

#include <Wire.h>
#include <SSD1306Wire.h>
#include <PMS.h>

// HardwareSerial Serial2(2);

#define I2C_SDA D2
#define I2C_SCL D1

PMS pms(Serial);  // PMS7003 sensor instance
PMS::DATA data;

/*
Initialize the OLED display using Wire library
display(0x3c, SDA, SCL) (optional)
*/
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);

uint16_t pm1;
uint16_t pm2_5;
uint16_t pm10;
String header="Home Air Monitoring Station";

void setup()
{
  delay(3000);  // wait 3 seconds till program begins
  Serial.begin(9600);  // GPIO1, GPIO3 (ESP32 TX/RX pins)
  // Serial2.begin(9600);  // GPIO17 (ESP32 TX2 pin)

  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop()
{
    display.clear();  // clear the display
  
    // write date & time to oled display
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.drawString(0, 0, header);
    display.drawHorizontalLine(0, 15, display.getStringWidth(header));
    
    Serial.println("Data:");
  
    // read from PMS7003
    if (pms.readUntil(data, 5000))
    {
      pm1 = data.PM_AE_UG_1_0;
      pm2_5 = data.PM_AE_UG_2_5;
      pm10 = data.PM_AE_UG_10_0;

      if (!isnan(pm1))
      {
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_1_0);
        display.drawString(0, 20, "PM 1.0   " + String(data.PM_AE_UG_1_0) + " [ug/m3]");
      }

      if (!isnan(pm2_5))
      {
        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(data.PM_AE_UG_2_5);
        display.drawString(0, 30, "PM 2.5   " + String(data.PM_AE_UG_2_5) + " [ug/m3]");
      }

      if (!isnan(pm1))
      {
        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_10_0);
        display.drawString(0, 40, "PM 10.0    " + String(data.PM_AE_UG_10_0) + " [ug/m3]");
      }
    }
    
    Serial.println();
    display.display();
    
    delay(10000);
  
}
