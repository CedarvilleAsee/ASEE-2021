#ifndef PINS
#define PINS

//display
const int CS    = PC13;
const int CLOCK = PC14;
const int DIN   = PC15;

// Buttons
const int BUTTON_2 = PB4;//Go red button
const int BUTTON_1 = PB5;//Go blue button

// Wheels
const int WHEEL_DIR_LB = PB14;//BIN1
const int WHEEL_DIR_LF = PB15;//BIN2
const int WHEEL_DIR_RB = PB13;//AIN1
const int WHEEL_DIR_RF = PB12;//AIN2

const int WHEEL_SPEED_L = PB1; //PWMB
const int WHEEL_SPEED_R = PB0; //PWMA

// Line sensors
//left to right is 7 to 0
const int LINE_SENSOR[] = {PA4, PA5, PA6, PA7, PB3, PA15, PB10, PB11};

//Servos
const int LINE_SENSOR_SERVO = PB10;
const int LAUNCHER_SERVO    = PB11;

//Other sensors
const int DISTANCE_SENSOR   = PB9;


#endif
