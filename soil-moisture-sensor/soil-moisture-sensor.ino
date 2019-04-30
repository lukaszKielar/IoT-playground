// Sketch allows to read soil moisture

#define ANALOG_PIN A0

// declare type of variables
float moisture_percentage;
int sensor_analog;

void setup()
{
  Serial.begin(9600);
  delay(500); // wait 5 seconds till user open Serial Monitor
  
}

void loop()
{
  sensor_analog = analogRead(ANALOG_PIN);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );

  if (moisture_percentage < 0)
  {
    moisture_percentage = 0;
  }
  
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.println(" %");
  delay(1000);
}

