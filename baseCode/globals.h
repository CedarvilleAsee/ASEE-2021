#ifndef GLOBALS
#define GLOBALS

//state control
int state = 0;
int subState = 0;
float timeInState = 0;
float stateTimer = 0;
bool otherHasShot = false;

enum color {Red, Blue};
color role = Red;
color order = Red;

//gate state
unsigned long int blueTime = 0;
unsigned long int redTime = 0;
unsigned long int myTime = 0;

//Line following
int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
int sensorCounter = 0;

//booleans
bool turning = false;
bool pickingUp = false;
bool atWall = false;
bool doPickup = false;

bool atNextLine = false;

//delta time global
unsigned long CurrentTime = 0.0;

//hardware objects
PT6961 SevenSeg(DIN, CLOCK, CS);
Adafruit_SSD1306 OledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(7, 8);
Servo lineSensorServo;
Servo launcherServo;


#endif
