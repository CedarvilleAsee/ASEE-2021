#ifndef PINS
#define PINS

//7 segment display
const int CS    = 44;
const int CLOCK = 46;
const int DIN   = 42;

// Buttons
const int BUTTON_2 = 32;//A0 Go red button
const int BUTTON_1 = 30;//A1 Go blue button

// Wheels
const int WHEEL_DIR_LB = 26;//BIN1
const int WHEEL_DIR_LF = 28;//BIN2
const int WHEEL_DIR_RB = 36;//AIN1
const int WHEEL_DIR_RF = 34;//AIN2
const int H_BRIDGE_ENABLE = 33;

const int WHEEL_SPEED_L = 7; //PWMB
const int WHEEL_SPEED_R = 8; //PWMA

// Line sensors
//left to right is 7 to 0
const int LINE_SENSOR[] = {51, 49, 47, 45, 43, 41, 39, 37};

//Servos
const int LINE_SENSOR_SERVO = 13;
const int LAUNCHER_SERVO    = 12;

//Other sensors
const int DISTANCE_SENSOR   = 9;


#endif
