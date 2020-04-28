/****
 * 
 * 
 */
#include <Wire.h>
#include <Adafruit_Sensor.h>
//thermophile sensor for temperature
#include "Adafruit_TMP006.h"
#include <iostream>
#include <fstream>
using namespace std;



//biosensor
#include "SparkFun_Bio_Sensor_Hub_Library.h"

//Initializes the thermophile adafruit sensor
Adafruit_TMP006 tmp006;

// Reset pin, MFIO pin, for the bio-sensor
int resPin = 4;
int mfioPin = 5;
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); 

bioData body;
//^^^^^^^^^^
//Gives access to the following fields
// body.confidence - Confidence in the heartrate value
// body.oxygen     - Blood oxygen level
// body.status     - Has a finger been sensed?

/*
 Summary of hardware connections for bio-sensor
 SDA -> SDA
 SCL -> SCL
 RESET -> PIN 4
 MFIO -> PIN 5
*/
ofstream file_;
void setup() {
  Serial.begin(9600);

  //starts the Adafruit thermophile sensor
    if (! tmp006.begin()) {
    Serial.println("No sensor found");
    while (1);
  }
  Serial.println("Send s to enter sleep mode, or w to wake up.  Measurements are not updated while asleep!");

  //Setup for the bio_sensor
    Wire.begin();
  int result = bioHub.begin();
  if (result == 0) // Zero errors!
    Serial.println("Sensor started!");
  else
    Serial.println("Could not communicate with the sensor!!!");
 
  Serial.println("Configuring Sensor...."); 
  int error = bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
  if(error == 0){ // Zero errors!
    Serial.println("Sensor configured.");
  }
  else {
    Serial.println("Error configuring sensor.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up. 
  Serial.println("Loading up the buffer with data....");

  delay(4000); 
}

void loop() {


  //Checks if thermophile is on
  checkMode();
  // Grab temperature measurements
  float objt = tmp006.readObjTempC();
  body = bioHub.readBpm(); //readBpm() is saved to body biodata 
  
  ofstream file_;
  file_.open("biostats");
  file_ << "Obj Temp: " + objt;
  file_ << "Heart Rate: " + body.heartRate;
  file_ << "Oxygen: " + body.oxygen;
  file_.close(); 
  delay(60000); // 1 minute per reading

}

//Checks whether adafruit thermophile sensor is on sleep or awake
void checkMode(){
    while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'w') {
      Serial.println("Waking up!");
      tmp006.wake();
    }
    else if (c == 's') {
      Serial.println("Going to sleep!");
      tmp006.sleep();
    }
  }
}
