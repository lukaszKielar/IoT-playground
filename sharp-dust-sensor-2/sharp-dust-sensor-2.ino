#define PIN_DATA_OUT A0              //Connect the IO port of the GP2 sensor analog A0 output
#define PIN_LED_VCC 2                    //The pin in the GP2 sensor that supplies power to the internal Led
const int DELAY_BEFORE_SAMPLING = 280; //Waiting time before sampling
const int DELAY_AFTER_SAMPLING = 40;     //Wait time after sampling
const int DELAY_LED_OFF = 9680;                //Intervals

/**
 * Read the GP2 sensor output voltage
*/
double getOutputV() {
  digitalWrite(PIN_LED_VCC, LOW);
  delayMicroseconds(DELAY_BEFORE_SAMPLING);
  double analogOutput = analogRead(PIN_DATA_OUT);
  delayMicroseconds(DELAY_AFTER_SAMPLING);
  digitalWrite(PIN_LED_VCC, HIGH);
  delayMicroseconds(DELAY_LED_OFF);
  //Arduino analog read value range is 0 ~ 1023, the following conversion is 0 ~ 5v
  double outputV = analogOutput / 1024 * 5;
  return outputV;
}

/**
   Calculate the dust density based on the output voltage
*/
double getDustDensity(double outputV) {
  //Output voltage and dust density conversion formula: ug / m3 = (V - 0.9) / 5 * 1000
  double ugm3 = (outputV - 0.9) / 5 * 1000;
  //Remove the undetected range
  if (ugm3 < 0) {
    ugm3 = 0;
  }
  return ugm3;
}

/**
   Calculate AQI based on dust density
   Environmental air quality index (AQI) technical requirements (Trial)
 */
double getAQI(double ugm3) {
  double aqiL = 0;
  double aqiH = 0;
  double bpL = 0;
  double bpH = 0;
  double aqi = 0;
  //According to the correspondence between pm2.5 and aqi were calculated aqi
  if (ugm3 >= 0 && ugm3 <= 35) {
    aqiL = 0;
    aqiH = 50;
    bpL = 0;
    bpH = 35;
  } else if (ugm3 > 35 && ugm3 <= 75) {
    aqiL = 50;
    aqiH = 100;
    bpL = 35;
    bpH = 75;
  } else if (ugm3 > 75 && ugm3 <= 115) {
    aqiL = 100;
    aqiH = 150;
    bpL = 75;
    bpH = 115;
  } else if (ugm3 > 115 && ugm3 <= 150) {
    aqiL = 150;
    aqiH = 200;
    bpL = 115;
    bpH = 150;
  } else if (ugm3 > 150 && ugm3 <= 250) {
    aqiL = 200;
    aqiH = 300;
    bpL = 150;
    bpH = 250;
  } else if (ugm3 > 250 && ugm3 <= 350) {
    aqiL = 300;
    aqiH = 400;
    bpL = 250;
    bpH = 350;
  } else if (ugm3 > 350) {
    aqiL = 400;
    aqiH = 500;
    bpL = 350;
    bpH = 500;
  }
  //formula: aqi = (aqiH - aqiL) / (bpH - bpL) * (desity - bpL) + aqiL;
  aqi = (aqiH - aqiL) / (bpH - bpL) * (ugm3 - bpL) + aqiL;
  return aqi;
}

/**
   According to aqi access level description
*/
String getGradeInfo(double aqi) {
  String gradeInfo;
  if (aqi >= 0 && aqi <= 50) {
    gradeInfo = String("Perfect");
  } else if (aqi > 50 && aqi <= 100) {
    gradeInfo = String("Good");
  } else if (aqi > 100 && aqi <= 150) {
    gradeInfo = String("Mild polluted");
  } else if (aqi > 150 && aqi <= 200) {
    gradeInfo = String("Medium polluted");
  } else if (aqi > 200 && aqi <= 300) {
    gradeInfo = String("Heavily polluted");
  } else if (aqi > 300 && aqi <= 500) {
    gradeInfo = String("Severely polluted");
  } else {
    gradeInfo = String("Broken roof!!!");
  }
  return gradeInfo;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DATA_OUT, INPUT); //Defined as input (ADC reads 1analog)
  pinMode(PIN_LED_VCC, OUTPUT); //Defined as output
}

void loop() {
  double outputV = getOutputV();                //Sampling Get the output voltage
  double ugm3 = getDustDensity(outputV); //Calculate the dust concentration
  double aqi = getAQI(ugm3);                       //Calculate aqi
  String gradeInfo = getGradeInfo(aqi);        //Calculate the level

  //Print to the serial port
  Serial.println(String("outputV=") + outputV + "\tug/m3=" + ugm3 + "\tAQI=" + aqi + "\tgradeInfo=" + gradeInfo);

  //The next detection is performed every 1 second
  delay(1000);
}
