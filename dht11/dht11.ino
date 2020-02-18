/*
DHT11 sketch
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include "DHT.h"

#define DHTPIN D2  // digital pin we're connecting to
#define DHTTYPE DHT11  // sensor type

DHT dht(DHTPIN, DHTTYPE);  // initialize senor

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT11 temperature and humidity readings");

  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // humidity readings
  if(!isnan(h))
  {
    Serial.print("Humidity [%]: ");
    Serial.println(h);
  }
  else
  {
    Serial.println("Failed to read humidity!");
  }

  // temperature readings
  if(!isnan(t))
  {
    Serial.print("Temperature [*C]: ");
    Serial.println(t);
  }
  else
  {
    Serial.println("Failed to read temperature!");
  }
  Serial.println();
  delay(4000);  // delay 4 seconds before next reading
}
