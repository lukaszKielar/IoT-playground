/*
ESP8266, DHT11, DS18B20, ThingSpeak
Copyright 2018, Łukasz Kielar, All rights reserved.
https://github.com/lukaszKielar
*/

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#define DHTPIN D2  // digital pin we're connecting to DHT11
#define DHTTYPE DHT11  // sensor type
#define ONE_WIRE_BUS D5  // digital pin we're connecting to DS18B20

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float dht_t, dht_h, ds18b20_t;

const char* ssid = "SSID";  // SSID (Name of your WiFi)
const char* password = "PASSWORD";  // WiFi password

const char* host = "api.thingspeak.com";
String api_key = "API_KEY";  // API key for ThingSpeak channel
const int httpPort = 80;

void setup()
{
  Serial.begin(9600);

  dht.begin();
  sensors.begin();

  Connect_to_Wifi();
}

void loop()
{
  String data_to_send = api_key;

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
    data_to_send += "&field1=";
    data_to_send += String(ds18b20_t);
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
    data_to_send += "&field2=";
    data_to_send += String(dht_t);
  }
  else
  {
    Serial.println("Failed to read DHT11 humidity!");
  }

  // DHT11 temperature readings
  if(!isnan(dht_t))
  {
    Serial.print("Temperature [*C]: ");
    Serial.print(dht_t);
    Serial.println(" (DHT11)");
    data_to_send += "&field3=";
    data_to_send += String(dht_h);
  }
  else
  {
    Serial.println("Failed to read DHT11 temperature!");
  }

  Serial.println();
  data_to_send += "\r\n\r\n";

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }

  else
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);
    delay(1000);
  }

  client.stop();
  delay(20000);  // delay 20 seconds before next reading
}

void Connect_to_Wifi()
{
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  Serial.print("Wait for WiFi... ");

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
