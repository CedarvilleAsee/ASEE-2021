#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include "FileManager.h"

//radio variables
bool radioNumber = 0;
byte addresses[][6] = {"1Node", "2Node"};

void setup() {

  //initialize displays
  SevenSeg.initDisplay();
  InitOled(OledDisplay);
  SendString(OledDisplay, "Display initialized!");
  SevenSeg.sendNum(1234, 1);

  // initialize line sensors
  for (int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }

  //initialize puck sensor
  pinMode(DISTANCE_SENSOR,INPUT);

  //initialize servos
  lineSensorServo.attach(LINE_SENSOR_SERVO);
  launcherServo.attach(LAUNCHER_SERVO);

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

  //Set servos to default positions
  launcherServo.write(LAUNCHER_HOLD);
  lineSensorServo.write(LINE_SERVO_DOWN);

  //checkSensorValidity();//check distance sensor------------------------------------------------------------------------------------------------------
}

void loop() {
  SevenSeg.sendNum(state + (subState * 100));
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
    WriteToWheels(0, 0);
      if (role == Red) {
        if (myTime == 0) { //get timing if not already done
          //SetupStateRed();------------------------------------------------------------------------------------------------------
          //SetupStateBlue();-----------------------------------------------------------------------------------------------------
          //RadioSend(GATE_STATE);------------------------------------------------------------------------------------------------
          //myTime = redTime;-----------------------------------------------------------------------------------------------------
          myTime = millis();
        }
        else { //wait for buttons after getting timing
          bool b1 = !digitalRead(BUTTON_1);
          bool b2 = !digitalRead(BUTTON_2);
          if (b1) {
            //RadioSend(GO_BLUE);------------------------------------------------------------------------------------------------
            EndState();
          }
          else if (b2) {
            //RadioSend(GO_RED);-------------------------------------------------------------------------------------------------
            EndState();
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
            EndState();
          }
          else if (message == GO_RED) {
            order = Red;
            EndState();
          }
        }
      }
      break;//----------------------------------------------exit when recieved start message or button press and black puck shot

    case 1://-----------------------------------------------turn to face first puck
      lineSensorServo.write(LINE_SERVO_DOWN);
      TurningState(STATE_TIME[1], true);
      break;//----------------------------------------------exit when maneuver is completed
    
    case 2://-----------------------------------------------drive blindly towards line
      ReadLine();
      if(subState == 0){//if we haven't found the line yet
        WriteToWheels(BLIND_SPEED, BLIND_SPEED);
        if(amountSeen > 1 ){//once we've found the line
          ReadLine();
          if(amountSeen > 1){
            subState = 1;
          }
        }
      }

      else if(subState == 1 && amountSeen == 0){//once we've driven past the line
        ReadLine();
        if(amountSeen == 0){
          subState = 3;
        }
      }
      
      else if(subState == 2 ){

        WriteToWheels(-TURN_SPEED,TURN_SPEED);
        ReadLine();
        if(firstLineIndex > 2){//once we've rotated into the line
          subState = 3;
        }
      }
      
      if(subState == 3){
        EndState();
      }
      break;//----------------------------------------------exit when line is found

    case 3://-----------------------------------------------turn to align with line
      TurningState(STATE_TIME[3], false);
      break;//----------------------------------------------exit when maneuver is completed

    case 4://-----------------------------------------------drive towards first puck
      ReadLine();
      LineFollow(FOLLOW_SPEED , LINE_STRICTNESS);

      if (GetPuckDist()) {
        EndState();
      }
      break;//----------------------------------------------exit when first puck is close enough to collect

    case 5://-----------------------------------------------collect first puck
      timeInState += DeltaTime();
      SetDelta();
      lineSensorServo.write(LINE_SERVO_UP);
      WriteToWheels(PICKUP_SPEED, PICKUP_SPEED);

      if (timeInState >= STATE_TIME[5]) {
        lineSensorServo.write(LINE_SERVO_DOWN);
        EndState();
      }
      break;//----------------------------------------------exit after puck is collected

    case 6://-----------------------------------------------turn
      TurningState(STATE_TIME[6], false, TURN_SPEED);
      break;//----------------------------------------------exit when maneuver is completed

    case 7://-----------------------------------------------drive to intercept line
      ReadLine();
      timeInState += DeltaTime();
      SetDelta();

      WriteToWheels(BLIND_SPEED, BLIND_SPEED);

      if (timeInState >= STATE_TIME[7] && amountSeen > 0) {
        EndState();
      }
      break;//----------------------------------------------exit when line is seen

    case 8://-----------------------------------------------drive towards second puck
      ReadLine();
      if(amountSeen == 0){
        WriteToWheels(FOLLOW_SPEED / 10,255);
      }
      else{
        LineFollow(FOLLOW_SPEED, LINE_STRICTNESS);
      }

      if (GetPuckDist()) {
        EndState();
      }
      break;//----------------------------------------------exit when puck is close enough to collect

    case 9://-----------------------------------------------collect second puck
      timeInState += DeltaTime();
      SetDelta();
      lineSensorServo.write(LINE_SERVO_UP);
      WriteToWheels(PICKUP_SPEED, PICKUP_SPEED);

      if (timeInState >= STATE_TIME[9]) {
        lineSensorServo.write(LINE_SERVO_DOWN);
        EndState();
      }
      break;//----------------------------------------------exit when puck is collected

    case 10://----------------------------------------------back up until line is found
      ReadLine();
      timeInState += DeltaTime();
      SetDelta();
      WriteToWheels(BACKUP_SPEED,BACKUP_SPEED);

      if(amountSeen > 1 && timeInState >= STATE_TIME[10]){
        EndState();
      }
      break;//-----------------------------------------------exit when line is found

    case 11://-----------------------------------------------turn towards center of track
      TurningState(STATE_TIME[11], false);
      break;//-----------------------------------------------exit when maneuver is completed

    case 12://-----------------------------------------------drive to center of track
      ReadLine();
      timeInState += DeltaTime();
      SetDelta();

      WriteToWheels(BLIND_SPEED,BLIND_SPEED);

      if (timeInState >= STATE_TIME[12]) {
        EndState();
      }
      break;//----------------------------------------------exit when in front of goal

    case 13://----------------------------------------------turn to face goal
      TurningState(STATE_TIME[13],true);
      break;
      
    case 14://----------------------------------------------drive up to line
      ReadLine();
      WriteToWheels(BLIND_SPEED,BLIND_SPEED);

      if(amountSeen > 1){
        EndState();
      }
      break;//-----------------------------------------------exit when line is found

    case 15://-----------------------------------------------shoot when able
      lineSensorServo.write(LINE_SERVO_UP);
      if (role == order || otherHasShot) {
        if (GateSafeToLaunch) {
          launcherServo.write(LAUNCHER_SHOOT2);
          //send shot message on exit
          //RadioSend(HAS_SHOT);------------------------------------------------------------------------------------------------------
          EndState();
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
    case 16://-----------------------------------------------time out corner
      WriteToWheels(0, 0); //stand still
      break;//----------------------------------------------there is no escape, eternal doom
  }
}

//display current state
void ShowState(){
  SevenSeg.sendNum(state + (subState * 100));
  SendString(OledDisplay,"Current state:" + (state + (subState * 100)));
}

//increment state and reset necessary variables
void EndState(){
  timeInState = 0;
  stateTimer = 0;
  subState = 0;
  state++;
  ShowState();
  SetDelta();
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

//Checks if the puck is close enough to collect
bool GetPuckDist(){
  return !digitalRead(DISTANCE_SENSOR);
}

void TurningState(unsigned long int turnTime, bool dir, int turnSpeed) {
  timeInState += DeltaTime();
  SetDelta();

  if (dir) {
    WriteToWheels(turnSpeed, -turnSpeed);
  }
  else {
    WriteToWheels(-turnSpeed, turnSpeed);
  }

  if (timeInState > turnTime) {
    EndState();
  }
}

void TurningState(unsigned long int turnTime, bool dir){
  TurningState(turnTime,dir,TURN_SPEED);
}

void GentleTurningState(unsigned long int turnTime, bool dir) {
  timeInState += DeltaTime();
  SetDelta();

  if (dir) {
    WriteToWheels(TURN_SPEED, 0);
  }
  else {
    WriteToWheels(0, TURN_SPEED);
  }

  if (timeInState > turnTime) {
    EndState();
  }
}
