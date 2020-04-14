/****
 * 
 * 
 */
#include <Wire.h>
#include <Adafruit_Sensor.h>
//thermophile sensor for temperature
#include "Adafruit_TMP006.h"
//biosensor
#include "SparkFun_Bio_Sensor_Hub_Library.h"
//bme sensor for humidity
#include <SPI.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

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
  
  // Grab temperature measurements and print them.
  float objt = tmp006.readObjTempC();
  Serial.print("Object Temperature: "); Serial.print(objt); Serial.println("*C");
  float diet = tmp006.readDieTempC();
  Serial.print("Die Temperature: "); Serial.print(diet); Serial.println("*C");

   if (objt > 101){
    Serial.print("User temperature is too high");
    Serial.print("Checking pupils for fatigue");
    bool pupilsFatigued = false;
    if (pupilsFatigued)
    { //Uses pupil dilation to decide if further action is needed.

        //Bio-sensor readings
        body = bioHub.readBpm(); //readBpm() is saved to body biodata 

        if (body.heartRate > 180 or body.oxygen < 90){
          // User is too fatigued and must stop
        }
      //  Serial.println(body.confidence); 
        Serial.println(body.status); 
   }
   }
  



   
  delay(4000); // 4 seconds per reading for 16 samples per reading

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
