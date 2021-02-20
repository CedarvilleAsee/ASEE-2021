#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "lineFollowing.h"
#include "turning.h"
#include "subStates.h"
#include "Time.h"
#include "GateTiming.h"
#include <SPI.h>
#include "RF24.h"
#include <ComponentObject.h>                //Used for the range sensors
#include <RangeSensor.h>                    //Used for the range sensors
#include <SparkFun_VL53L1X.h>               //Used for the range sensors
#include <vl53l1x_class.h>                  //Used for the range sensors
#include <vl53l1_error_codes.h>             //Used for the range sensors
#include <Wire.h>
#include "SparkFun_VL53L1X.h"

bool radioNumber = 0;
RF24 radio(7, 8);
byte addresses[][6] = {"1Node", "2Node"};

//Servo lineSensorServo;------------------------------------------------------------------------------------------------------
//Servo launcherServo;------------------------------------------------------------------------------------------------------

void setup() {
  //display.initDisplay();
  //display.sendNum(1234, 1);
  // initialize line sensors
  for (int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }

  //initialize servos
  //lineSensorServo.attach(LINE_SENSOR_SERVO);------------------------------------------------------------------------------------------------------
  //launcherServo.attach(LAUNCHER_SERVO);------------------------------------------------------------------------------------------------------

  Serial.begin(115200);

  //Open i2c for distance sensors
  Wire.begin();

  //initialize radio
  /*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
    } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    }
    radio.startListening();
  */
  // initialize motor controllers
  pinMode(WHEEL_DIR_LB, OUTPUT);
  pinMode(WHEEL_DIR_LF, OUTPUT);
  pinMode(WHEEL_DIR_RB, OUTPUT);
  pinMode(WHEEL_DIR_RF, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  //initialize buttons
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  //afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work

  //checkSensorValidity();//check distance sensor------------------------------------------------------------------------------------------------------
}

void loop() {
  //display.sendNum(state);
  /*
     State setup:
     case n:
      setup functions

      state action logic

      exit condition
      break;
  */

  switch (state) {
    case 0://-----------------------------------------------receive or send goalie timing & role
      if (role == Red) {
        if (myTime == 0) { //get timing if not already done
          //SetupStateRed();------------------------------------------------------------------------------------------------------
          //SetupStateBlue();------------------------------------------------------------------------------------------------------
          //RadioSend(GATE_STATE);------------------------------------------------------------------------------------------------------
          //myTime = redTime;------------------------------------------------------------------------------------------------------
          myTime = millis();
        }
        else { //wait for buttons after getting timing
          bool b1 = !digitalRead(BUTTON_1);
          bool b2 = !digitalRead(BUTTON_2);
          if (b1) {
            //RadioSend(GO_BLUE);------------------------------------------------------------------------------------------------------
            state++;
            timeInState = 0;
            SetDelta();
          }
          else if (b2) {
            //RadioSend(GO_RED);------------------------------------------------------------------------------------------------------
            state++;
            timeInState = 0;
            SetDelta();
          }
        }
      }

      else {
        if (myTime == 0) { //get timing if not already done
          //RadioRead();------------------------------------------------------------------------------------------------------
          myTime = millis();
        }
        else { //wait for buttons after getting timing
          int message = RadioRead();
          if (message == GO_BLUE) {
            order = Blue;
            state++;
          }
          else if (message == GO_RED) {
            order = Red;
            state++;
          }
        }
      }
      break;//----------------------------------------------exit when recieved start message or button press and black puck shot

    case 1://-----------------------------------------------turn to face first puck
      TurningState(STATE_TIME[1], true);
      break;//----------------------------------------------exit after turn is completed

    case 2://-----------------------------------------------drive
      WriteToWheels(BLIND_SPEED, BLIND_SPEED);

      if (GetPuckDist()) {
        state++;
      }
      break;//----------------------------------------------exit when first puck is close enough to collect

    case 3://-----------------------------------------------collect first puck
      timeInState += DeltaTime();
      SetDelta();

      WriteToWheels(PICKUP_SPEED, PICKUP_SPEED);

      if (timeInState >= STATE_TIME[3]) {
        state++;
        timeInState = 0;
      }
      break;//----------------------------------------------exit after puck is collected

    case 4://-----------------------------------------------turn
      TurningState(STATE_TIME[4], false);
      break;//----------------------------------------------exit when maneuver is completed

    case 5://-----------------------------------------------drive to intercept line
      ReadLine();
      timeInState += DeltaTime();
      SetDelta();

      WriteToWheels(BLIND_SPEED, BLIND_SPEED);

      if (timeInState >= STATE_TIME[5] && amountSeen > 0) {
        state++;
        timeInState = 0;
      }
      break;//----------------------------------------------exit when line is seen

    case 6://-----------------------------------------------line follow
      ReadLine();

      LineFollow(FOLLOW_SPEED, LINE_STRICTNESS);

      if (GetPuckDist()) {
        state++;
      }
      break;//----------------------------------------------exit when puck is close enough to collect

    case 7://-----------------------------------------------collect second puck
      timeInState += DeltaTime();
      SetDelta();

      WriteToWheels(PICKUP_SPEED, PICKUP_SPEED);

      if (timeInState >= STATE_TIME[7]) {
        state++;
        timeInState = 0;
      }
      break;//----------------------------------------------exit when puck is collected

    case 8://-----------------------------------------------drive in front of goal
      ReadLine();
      timeInState += DeltaTime();

      LineFollow(FOLLOW_SPEED, LINE_STRICTNESS);

      if (timeInState >= STATE_TIME[8]) {
        state++;
        timeInState = 0;
      }
      break;//----------------------------------------------exit when maneuver is completed

    case 9://-----------------------------------------------turn to face goal
      TurningState(STATE_TIME[9], true);
      break;//----------------------------------------------exit when maneuver is completed

    case 10://-----------------------------------------------shoot when able
      if (role == order || otherHasShot) {
        if (GateSafeToLaunch) {
          //launcherServo.write(LAUNCHER_SHOOT2);------------------------------------------------------------------------------------------------------
          //send shot message on exit
          //RadioSend(HAS_SHOT);------------------------------------------------------------------------------------------------------
          state++;
        }
      }
      else {
        otherHasShot = true;
        /*------------------------------------------------------------------------------------------------------
          if (RadioRead() == HAS_SHOT) {
          otherHasShot = true;
          }
        */
      }
      break;//----------------------------------------------exit when shot

    default:
    case 11://-----------------------------------------------time out corner
      WriteToWheels(0, 0); //stand still
      break;//----------------------------------------------there is no escape, eternal doom
  }
}

void TurningState(unsigned long int turnTime, bool dir) {
  timeInState += DeltaTime();
  SetDelta();

  if (dir) {
    WriteToWheels(TURN_SPEED, -TURN_SPEED);
  }
  else {
    WriteToWheels(-TURN_SPEED, TURN_SPEED);
  }

  if (timeInState > turnTime) {
    state++;
    timeInState = 0;
  }
}

void RadioSend(int message) {
  radio.stopListening();

  if (!radio.write( &message, sizeof(int) )) {
    Serial.println(F("failed to send"));
  }

  radio.startListening();
}

int RadioRead() {
  int temp = -1;
  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &temp, sizeof(int));
      return temp;
    }
  }
  return temp;
}

