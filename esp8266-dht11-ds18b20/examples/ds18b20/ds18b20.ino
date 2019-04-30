/*
ESP8266, DS18B20, Serial Monitor
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D5

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float celcius;

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  celcius=sensors.getTempCByIndex(0);
  Serial.print("Temperature [*C] ");
  Serial.print(Celcius);
  Serial.println(" (DS18B20)");
  delay(1000);
}
