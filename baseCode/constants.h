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
const int STATE_TIME[17] = {
  /*State 0 */0,         /*Receive or send goalie timing & role*/         /*No affect, exits when button is pressed or go signal is received*/
  /*State 1 */375,       /*Turn to face first puck*/                      /*Time (in ms) to turn for*/
  /*State 2 */0,         /*Drive blindly towards line*/                   /*No affect, exits when the line is found*/
  /*State 3 */300,       /*Turn to align with line*/                      /*Time (in ms) to turn for*/
  /*State 4 */0,         /*Drive towards first puck*/                     /*No affect, exits when the puck is found*/
  /*State 5 */1000,      /*Collect first puck*/                           /*Time (in ms) to drive forwards for puck collection*/
  /*State 6 */1000,      /*Turn to face second puck*/                     /*Time (in ms) to turn for*/
  /*State 7 */500,       /*Drive to intercept line*/                      /*Time to ignore line sensor data when crossing track*/
  /*State 8 */0,         /*Drive towards second puck*/                    /*No affect, exits when the puck is found*/
  /*State 9 */1200,      /*Collect second puck*/                          /*Time (in ms) to drive forwards for puck collection*/
  /*State 10*/1000,      /*Back up until the line is found*/              /*minimum time (in ms) for backing up*/
  /*State 11*/450,       /*Turn towards center of track*/                 /*Time (in ms) to turn for*/
  /*State 12*/950,       /*Drive to center of track*/                     /*Time (in ms) to drive for*/
  /*State 13*/666,       /*turn to face goal*/                            /*Time (in ms) to turn for*/
  /*State 14*/0,         /*drive up to line*/                             /*No affect, exits after line is found*/
  /*State 15*/0,         /*Shoot when able*/                              /*No affect, exits after puck has been shot*/
  /*State 16*/0          /*Time out corner*/                              /*No affect, never exits*/
  };

//speed constants
const int TURN_SPEED      = 180;    //wheel speed when turning
const int BLIND_SPEED     = 190;    //wheel speed when travelling without line
const int FOLLOW_SPEED    = 160;    //wheel speed when following line
const int PICKUP_SPEED    = 100;    //wheel speed when picking up a puck
const int BACKUP_SPEED    = -150;   //wheel speed when backing up
const int LINE_STRICTNESS = 80;     //line follow tuning parameter

//servo angles
const int LAUNCHER_HOLD     = 25;   //hold both mousetraps
const int LAUNCHER_SHOOT1   = 60;   //fire mousetrap 1
const int LAUNCHER_SHOOT2   = 75;   //fire mousetrap 2
const int LINE_SERVO_UP     = 110;  //open front to collect puck
const int LINE_SERVO_DOWN   = 140;  //lower front to line follow

//message constants
const int GATE_STATE  = 0;
const int GO_BLUE     = 1;
const int GO_RED      = 2;
const int HAS_SHOT    = 3;

#endif
