#ifndef CONSTANTS

#define CONSTANTS

//gate timing constants
const int CLOSE_MARGIN = 0;
const int OPEN_MARGIN = 0;
const int RED_DISTANCE = 300;
const int BLUE_DISTANCE = 200;
const int TIME_OPEN = 3000;
const int TIME_CLOSED = 2000;

//state timing constants
const int STATE_TIME[10] = {
  /*State 0*/0,
  /*State 1*/400,
  /*State 2*/0,
  /*State 3*/1000,
  /*State 4*/300,
  /*State 5*/5000,
  /*State 6*/0,
  /*State 7*/1000,
  /*State 8*/2000,
  /*State 9*/500
  };

//speed constants
const int TURN_SPEED      = 50;     //wheel speed when turning
const int BLIND_SPEED     = 120;    //wheel speed when travelling without line
const int FOLLOW_SPEED    = 150;    //wheel speed when following line
const int PICKUP_SPEED    = 100;    //wheel speed when picking up a puck
const int LINE_STRICTNESS = 20;     //line follow tuning parameter

//servo angles
const int LAUNCHER_HOLD     = 20;   //hold both mousetraps
const int LAUNCHER_SHOOT1   = 30;   //fire mousetrap 1
const int LAUNCHER_SHOOT2   = 35;   //fire mousetrap 2
const int LINE_SERVO_UP     = 0;    //open front to collect puck
const int LINE_SERVO_DOWN   = 100;  //lower front to line follow


//message constants
const int GATE_STATE  = 0;
const int GO_BLUE     = 1;
const int GO_RED      = 2;
const int HAS_SHOT    = 3;

//direction constants
const char L = 'l';
const char R = 'r';
const char F = 'f';
const char B = 'b';
const char E = '0';

// Dasch constants
const bool WHEEL_FORWARDS   = true;
const bool WHEEL_BACKWARDS  = false;
#endif
