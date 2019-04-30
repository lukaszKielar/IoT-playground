/*
ESP8266, DHT11, DS18B20, Serial Monitor
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN D2  // digital pin we're connecting to DHT11
#define DHTTYPE DHT11  // sensor type
#define ONE_WIRE_BUS D5  // digital pin we're connecting to DS18B20

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float dht_t, dht_h, ds18b20_t;

void setup()
{
  Serial.begin(9600);

  dht.begin();
  sensors.begin();
}

void loop()
{
  dht_h = dht.readHumidity();
  dht_t = dht.readTemperature();
  sensors.requestTemperatures();
  ds18b20_t = sensors.getTempCByIndex(0);

  // DS18B20 temperature reading
  if(!isnan(ds18b20_t))
  {
    Serial.print("Temperature [*C]: ");
    Serial.print(ds18b20_t);
    Serial.println(" (DS18B20)");
  }
  else
  {
    Serial.println("Failed to read DS18B20 temperature!");
  }

  // DHT11 humidity reading
  if(!isnan(dht_h))
  {
    Serial.print("Humidity [%]: ");
    Serial.print(dht_h);
    Serial.println(" (DHT11)");
  }
  else
  {
    Serial.println("Failed to read DHT11 humidity!");
  }

  // DHT11 temperature reading
  if(!isnan(dht_t))
  {
    Serial.print("Temperature [*C]: ");
    Serial.print(dht_t);
    Serial.println(" (DHT11)");
  }
  else
  {
    Serial.println("Failed to read DHT11 temperature!");
  }

  Serial.println();
  delay(20000);  // delay 20 seconds before next readings
}
