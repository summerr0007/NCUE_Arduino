#include "TRSensors.h"

#define NUM_SENSORS 5
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  Serial.begin(9600);
  Serial.println("TRSensor example");
  //校準的過程中需要把車子放在白色平面上
  for (int i = 0; i < 40; i++)  // make the calibration take about 10 seconds
  {
    trs.calibrate();       // reads all sensors 10 times
  }
  Serial.println("calibrate done");
  // print the calibration minimum values measured when emitters were on
  //找出反射率的最大最小值
  Serial.print("MIN:");
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(trs.calibratedMin[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  Serial.print("MAX:");
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(trs.calibratedMax[i]);
    Serial.print(' ');
  }
  
  Serial.println();
  delay(500);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  // trs.AnalogRead(sensorValues);
  // trs.readCalibrated(sensorValues);
  unsigned int position = trs.readLine(sensorValues);
  trs.AnalogRead(sensorValues);
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  //印出數值
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
  //印出判斷後結果
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    unsigned int standard = (trs.calibratedMax[i] + trs.calibratedMin[i]) / 2 / 2;
    if(sensorValues[i] > standard)
      Serial.print("W");
     else
      Serial.print("B");
    Serial.print('\t');
  }
  Serial.println();

  Serial.println(position); // comment this line out if you are using raw values

  delay(750);
}
