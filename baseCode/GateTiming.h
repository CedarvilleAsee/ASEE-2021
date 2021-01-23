#include <ComponentObject.h>                //Used for the range sensors
#include <RangeSensor.h>                    //Used for the range sensors
#include <SparkFun_VL53L1X.h>               //Used for the range sensors
#include <vl53l1x_class.h>                  //Used for the range sensors
#include <vl53l1_error_codes.h>             //Used for the range sensors

SFEVL53L1X redGateSensor;
SFEVL53L1X blueGateSensor;

boolean GateIsOpen()
{
  return (((millis() - blueTime) % 5000) > 2000);
}

boolean GateSafeToLaunch()
{
  if((((millis() - redTime) % 5000) < (CLOSE_MARGIN + 2000)) || (((millis() - redTime) % 5000) > (5000 - OPEN_MARGIN)))          
  {  
    return false;
  }
  return true;
}

void checkSensorValidity()
{
  Serial.println(F("Checking sensor validity..."));
  redGateSensor.startRanging();
  blueGateSensor.startRanging();
  delay(500);
  if(redGateSensor.getDistance() == 0)      //If sensor always reads 0, it is messed up
  {
    Serial.println(F("Red sensor reading 0 always. Check wiring and sensor clearance"));
    while(1)
    {
      //digitalWrite(redStatusLED, ((millis() % 100) > 50));         //Flash LED and stop program
    }
  }
  if(blueGateSensor.getDistance() == 0)    //If sensor always reads 0, it is messed up
  {
    while(1)
    {
      Serial.println(F("Blue sensor reading 0 always. Check wiring and sensor clearance"));
      //digitalWrite(blueStatusLED, ((millis() % 100) > 50));      //Flash LED and stop program
    }
  }
  Serial.println(F("Sensors appear valid!"));
}

void initializeSensors()
{
  Serial.println(F("Initializing sensors..."));
  if(redGateSensor.begin() != 0)        //Redgate.begin() returns 0 if sensor started properly
  {
    Serial.println(F("Red gate sensor failed to start. Check wiring?"));
    while(1)                      //Stop the program and flash the led fast.
    {
      //digitalWrite(redStatusLED, ((millis() % 100) > 50));
    }
  }
  Serial.println("Initializing complete!");
}

void SetupStateRed()
{
  Serial.println(F("Checking red gate state..."));
  redGateSensor.startRanging();                       //Enable reading from the sensor
  int distance = redGateSensor.getDistance();
  while(distance < RED_DISTANCE)                // Wait for the gate to open
  {
    distance = redGateSensor.getDistance();
  }
  while(distance > RED_DISTANCE)                // While gate is open
  {
    redTime = millis();
    distance = redGateSensor.getDistance();
  }
  redGateSensor.clearInterrupt();
  redGateSensor.stopRanging();
  Serial.println(F("Red gate state established!"));
}

void setupStateBlue()
{
  Serial.println(F("Checking blue gate state..."));
  blueGateSensor.startRanging();
  int distance = blueGateSensor.getDistance();
  while(distance < BLUE_DISTANCE)                // Wait for the gate to open
  {
    distance = blueGateSensor.getDistance();
  }
  while(distance > BLUE_DISTANCE)                // While gate is open
  {
    blueTime = millis();
    distance = blueGateSensor.getDistance();
  }
  Serial.println(F("Blue gate state established!"));
}


