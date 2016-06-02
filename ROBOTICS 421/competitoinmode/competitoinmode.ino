#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include <pin_magic.h>
#include <registers.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include <SparkFunAutoDriver.h>
#include <SparkFundSPINConstants.h>
#include <VarSpeedServo.h>
#include <Adafruit_GFX.h> // Adafruit graphics
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Wire.h>

//////define fishing points here
#define point1x 100
#define point1a 100
#define point2x 100
#define point2a 100
#define point3x 100
#define point3a 100
#define point4x 100
#define point4a 100
#define point5x 100
#define point5a 100
#define point6x 100
#define point6a 100

////////////////////////////////////////////////////
//////board game defines/////////
////////////////////////////////////////////////////
#define unloadAngle 0     //this is the angle at which the head is moved to in order to unload a fish
#define xunloadPos  25    //the step position of the x carriage for unloading 

/////////////////////////////////////////////
////inputs and output pins////
/////////////////////////////////////////////
#define homeflagpin 25
#define headPin 11
#define heightPin 10
#define pressurePin A4
#define startPin 24
#define fishPin 14

/////////////////////////////////////////////
////Stepper Control Variables////
/////////////////////////////////////////////
#define xCSPin 45                             //stepper driver chip select pin  
#define xBusyPin 46                           //stepper driver busypin      
#define xRSTPin 47                            //reset stepper driver chip pin
#define xAcc 600                              //acel in steps/sec/sec
#define xDec 1000                             //dec in steps/sec/sec
#define xVelMin 50                            //minimum steps /sec
#define xVelMax 500                           //max steps/sec
#define holdtorquepower 16                    //% of max torque for holding between movements (WARNING GENERATES HEAT!) 
#define xPosMin 1                             //minimum step position logically
#define xPosMax 750                           //maximum logical step position      

//Autodriver Instances
AutoDriver XStepper(xCSPin, xRSTPin, xBusyPin);       //create instance of stepper

/////////////////////////////////////////////
////Servo control variables////
/////////////////////////////////////////////
#define headPosMin 10                         //PWM minimum value for angle 0    (needed to remove shaking)
#define headPosMax 245                        //pwm maximum value for angle 180 (needed to remove shaking)  
#define headVel 50                            //velocity of head actuator 1-255

#define heightPosMin 10                       //PWM minimum value for angle 0    (needed to remove shaking)
#define heightPosMax 245                      //PWM MAXIMUM value for angle 0    (needed to remove shaking)
#define heightVel 100                         //velocity of HEIGHT actuator 1-255

//create servos
VarSpeedServo HeadAngle;                                              //SERVO FOR HEAD ANGLE
VarSpeedServo Height;                                                 //servo FOR HEIGHT

/////////////////////////////////////////////
//LCD Touchscreen over pins and variables //
/////////////////////////////////////////////
#define SD_CS 24

// These are the four touchscreen analog pins
#define YP A15
#define XM A12
#define YM A14
#define XP A13

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TFT_CS 31
#define TFT_DC 30

#define FRAME_X 65
#define FRAME_Y 11
#define FRAME_W 200
#define FRAME_H 200

#define BUTTONHEADER_X FRAME_X
#define BUTTONHEADER_Y FRAME_Y
#define BUTTONHEADER_W (FRAME_W)
#define BUTTONHEADER_H (FRAME_H/5)

#define BUTTONCOMPETITION_X FRAME_X
#define BUTTONCOMPETITION_Y (FRAME_Y+((FRAME_H/5)*2))
#define BUTTONCOMPETITION_W (FRAME_W)
#define BUTTONCOMPETITION_H (FRAME_H/5)

#define BUTTONDEBUG_X FRAME_X
#define BUTTONDEBUG_Y (FRAME_Y+((FRAME_H/5)*4))
#define BUTTONDEBUG_W (FRAME_W)
#define BUTTONDEBUG_H (FRAME_H/5)

boolean RecordOn = false;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);                   //last variable is resistance between X+ and X- pins.

//somethings for later
#define commlength 3

//init position variables
int targetpos[] = {0, 0, 0};                                         //XPOS IN STEPS, HEAD ANGLE, HEIGHT ACTUATOR
int currentpos[] = {0, 0, 0};                                       //STORE THE CURRENT POSITION HERE AFTER MOTOR IS NO LONGER BUSY
int reqpos[] = {0, 0, 0};
int deltapos[] = {0, 0, 0};
int iscrn = 0;                                                      //counter for displaying waiting period on display
String menuString = "";
int xx;
int yy;
int selected = 0;
char incomingByte = 0;
boolean debug = false;
const byte numChars = 20;
char receivedChars[numChars];
boolean newData = false;
int boardHeight = 95;    //this is the angle of the height servo at which the fishing rod just touches the top of the blue game board
int tableHeight = 160;   //this is the angle at which the robot should drop fish off
int fishtimeout = 4;
int start;

void setup() {
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  //  SPI.setDataMode(SPI_MODE1);

  //activate tft screen
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);                                     //blackscreen
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);                    // White on black
  tft.setTextWrap(true);                                             // Don't wrap text to next line
  tft.setTextSize(1);                                                // large letters
  tft.setRotation(1);                                                 // horizontal display


  //attach servos to ports
  HeadAngle.attach(headPin, headPosMin, headPosMax);                // attaches the servo on pin 10 to the servo object
  Height.attach(heightPin, heightPosMin, heightPosMax);
  tft.println("Servos attached");

  //start serial comm
  Serial.begin(9600);                                                // set up Serial library at 9600 bps
  Serial.println("Initializing!");                                  // prints hello with ending line break
  tft.println("Serial begin");

  //autodrive config
  XStepper.configSyncPin(BUSY_PIN, 0);                                  //sets pin to be busy check
  XStepper.configStepMode(STEP_FS);                                     //Sets single step mode add "_2^x" to the end where x is a number between 1-5
  XStepper.setMaxSpeed(xVelMax);                                        //max speed in rpm
  XStepper.setMinSpeed(xVelMin);                                        //min speed in rpm
  XStepper.setAcc(xAcc);                                                //sets accel in steps /min/min
  XStepper.setDec(xDec);                                                 //sets decel in steps /min/min
  XStepper.setOCThreshold(OC_2625mA);                                    //over current threshhold, do not change only allows certain mA values
  XStepper.setHoldKVAL(holdtorquepower);                                 //sets holding torque power to a % of max, might need to increase later
  XStepper.setLoSpdOpt(true);                                            //better control for low speed steps, might try this toggled on and off.
  XStepper.setPWMFreq(PWM_DIV_2, PWM_MUL_2);                             //PWM Frequency dividers and multipliers to keep the whine down
  XStepper.setSlewRate(SR_530V_us);                                      //Sets rate of increase in voltage between phases during a step, higher means more torque at large RPM.
  tft.println("Stepper Activated");
  zeroX();                                                                //Resets the x position
  delay(50);
  pinMode(fishPin, INPUT);
  pinMode(startPin, INPUT);

  modeselect();
  delay(1000);
  tft.fillScreen(ILI9341_BLACK);                                     //blackscreen

  iscrn = 0;                                                              //resets counter if inside waiting loop with display
  tft.setTextSize(0);
  tft.setTextColor(ILI9341_ORANGE);
  tft.setCursor(0, 0);
  tft.println("test");
  RoboMove(725, 90, 100);
  delay(5000);
  RoboMove(100, 10, 10);

}
void loop() {

  start = digitalRead(startPin);

  //wait for start of game
  while (start == 1)
  {
    tft.println("Waiting on start");
    delay(10);
    start = digitalRead(startPin);
    checkscreen();
  }


  //do something while the game is going.
  while (start == 0)
  {
    fishing(point1x, point1a);
    fishing(point2x, point2a);
    fishing(point3x, point3a);
    fishing(point4x, point4a);
    fishing(point5x, point5a);
    fishing(point6x, point6a);

  }

}
int zeroX() { //zeros the x axis of the robot with the micro switch

  pinMode(42, INPUT);
  while ( digitalRead(homeflagpin) == 0 );
  {
    XStepper.run(REV, 100);
  }
  while ( digitalRead(homeflagpin) == 1 );
  {
    XStepper.run(FWD, 20);
  }
  XStepper.hardStop();
  delay(100);
  tft.println("Zeroed");
  XStepper.resetPos();
  delay(500);
  XStepper.goTo(100);
  waitforX();
}

int checkpos() { //function to validate requested position move takes position in reqpos[] and puts it into targetpos[]

  tft.println();
  tft.print("Checking inputs:");
  for (int i = 0; i < 3; i++)
  {
    tft.print(reqpos[i]);
    tft.print(", ");
  }



  if (reqpos[0] < xPosMin or reqpos[0] > xPosMax)
  {
    if (selected == 4)
    {
      tft.println("X POS OUTSIDE BOUNDS");
    }

    reqpos[0] = currentpos[0];
    deltapos[0] = 0;
  }
  else
  {
    targetpos[0] = reqpos[0];
    deltapos[0] = currentpos[0] - targetpos[0];
  }
  if (reqpos[1] < 0 or reqpos[1] > 180)
  {
    if (selected == 4)
    {
      tft.println("HEAD ANGLE OUT OF BOUNDS");
    }

    reqpos[0] = currentpos[0];
    deltapos[1] = 0;
  }
  else
  {
    targetpos[1] = reqpos[1];
    deltapos[1] = currentpos[1] - targetpos[1];
  }
  if (reqpos[2] < 0 or reqpos[2] > 180)
  {
    if (selected == 4)
    {
      tft.println("HEIGHT ANGLE OUT OF BOUNDS");
    }

    reqpos[2] = currentpos[2];
    deltapos[2] = 0;
  }
  else
  {
    targetpos[2] = reqpos[2];
    deltapos[2] = currentpos[2] - targetpos[2];
  }
  if (deltapos[0] == 0 and deltapos[1] == 0 and deltapos[2] == 0)
  {
    if (selected == 4)
    {
      tft.println("No movement requested");
    }

  }
  if (selected == 4)
  {
    tft.print("Moving to: ");
    for (int n = 0; n < 3; n++)
    {
      tft.print(targetpos[n]);
      tft.print(", ");
    }
    tft.println();
  }

}

int shakey(int x) {
  // This function shakes the fishing rod to dislodge a fish the number of times which is passed

#define shakeyWaitTime 100    //the amount of time to allow the rod to move (less time is theoretically more jerky) 
#define fishHeight 20         //this is the difference in angle of the Height servo for the height of a fish. So we don't jam the fishing rod into the table

  //GO down to the table to set the fish on it.
  Height.write(tableHeight - fishHeight, 254);
  delay(200);

  for (int i = 0; i < x; i++)       //do it as many times as we're told
  {

    delay(shakeyWaitTime);
    Height.write(5, 250);                             //Raise the fishing rod.
    delay(shakeyWaitTime);
    Height.write(tableHeight, 250);
    if (selected == 4)
    {
      tft.print("Shakey!");
    }
    checkscreen();
  }                                                   //rinse
  tft.println();
  Height.write(5, 150);
  checkscreen();
}
int waitforX() {                  //waits for the stepper motor to no longer be busy before proceeding.
  if (selected == 4)
  {
    tft.print("Waiting on stepper");
  }
  int x = tft.getCursorX();
  int y = tft.getCursorY();
  iscrn = 0;                                                 //avoid a memory overflow
  while (XStepper.busyCheck() == 1)
  {
    if (selected == 4)
    {
      waiting(12, x, y);
    }
    else
    {
      delay(10);
    }
  }
  tft.println();
  checkscreen();

}
int RoboMove(int x, int headA, int heightA) {
  reqpos[0] = x;
  reqpos[1] = headA;
  reqpos[2] = heightA;
  checkpos();                             //checks the requested input to ensure that the position exists and doesn't cause a conflict


  Height.write(targetpos[2], heightVel);               // move servo to target position at height velocity defined above.
  HeadAngle.write(targetpos[1], headVel);              // move servo to target position at height velocity defined above.
  XStepper.goTo(targetpos[0]);                         //move stepper motor to the requested position. Stepper decides direction, accel and speed.
  for (int n = 0; n < 3; n++)
  {
    currentpos[n] = targetpos[n];                      //set the target position to the current posision post-move
    deltapos[n] = 0;                                   //reset the change in position array which tracks if something is changing or not.
  }
  waitforX();
  delay(100);
  checkscreen();
}
int fishing(int steps, int angle) {
  //  RoboMove(300, 90, 0);
  //  delay(50);

  tft.print("Fishing");

  RoboMove(steps, angle, (boardHeight - 5));

  delay(600);                                                              //settling time for the fishing rod

  int fishcount = 0;
  int temp;

  do
  {

    start = digitalRead(startPin);
    while (start == 1)
    {
      delay(100);
    }

    Height.write((boardHeight - 5), 250);
    HeadAngle.write((angle), headVel);
    delay(750);
    //fishing rod up and down
    Height.write((boardHeight + 25), 125);                                      // move servo to target position at height velocity defined above.
    delay(250);

    start = digitalRead(startPin);
    while (start == 1)
    {
      delay(100);
    }

    //fish pull
    Height.write(0, 250);                                      // move servo to target position at height velocity defined above.
    fishcount++;
    delay(1000);
    temp = digitalRead(fishPin);
  } while ((temp == LOW) && (fishcount < fishtimeout));

  //turn back
  start = digitalRead(startPin);
  while (start == 1)
  {
    delay(100);
  }
  RoboMove(25, 5, 5);                                       // move servo to target position at height velocity defined above.
  delay(250);
  shakey(4);
  start = digitalRead(startPin);
  while (start == 1)
  {
    delay(100);
  }
}

int checkscreen() {
  if (tft.getCursorY() > 180 ) {
    tft.fillScreen(ILI9341_BLACK);                                     //blackscreen

    iscrn = 0;                                                              //resets counter if inside waiting loop with display
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_ORANGE);
    tft.setCursor(0, 0);
    tft.println("Screen Cleared...");

  }
}
int waiting(int waitdiv, int endline, int line) {
  delay (5);
  iscrn++;
  if (iscrn > 30000) {
    iscrn = 0;
  }
  if (iscrn % waitdiv == 1)
  {
    tft.print(".");
    if (tft.getCursorX() > 310) {                                                 //if we reach the end of the screen clear the periods and reprint them.
      tft.fillRect(endline, line, 300, 7, ILI9341_BLACK); tft.setCursor(endline, line);
    }
  }
}

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void modeselect() {
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0, 0, 320, 240, ILI9341_WHITE);
  //header
  tft.fillRect(BUTTONHEADER_X, BUTTONHEADER_Y, BUTTONHEADER_W, BUTTONHEADER_H, ILI9341_BLACK);
  tft.setCursor(BUTTONHEADER_X + 4 , BUTTONHEADER_Y + (BUTTONHEADER_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Mode Select:");

  //competition
  tft.fillRect(BUTTONCOMPETITION_X, BUTTONCOMPETITION_Y, BUTTONCOMPETITION_W, BUTTONCOMPETITION_H, ILI9341_ORANGE);
  tft.setCursor(BUTTONCOMPETITION_X + 4 , BUTTONCOMPETITION_Y + (BUTTONCOMPETITION_H / 2));
  tft.println("Competition Mode");

  //debug
  tft.fillRect(BUTTONDEBUG_X, BUTTONDEBUG_Y, BUTTONCOMPETITION_W, BUTTONCOMPETITION_H, ILI9341_WHITE);
  tft.setCursor(BUTTONDEBUG_X + 4 , BUTTONDEBUG_Y + (BUTTONCOMPETITION_H / 2));
  tft.println("Debug Mode");
  drawFrame();


  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(15, 20);
  tft.println("T");
  tft.setCursor(15, 45);
  tft.println("E");
  tft.setCursor(15, 70);
  tft.println("A");
  tft.setCursor(15, 95);
  tft.println("M");
  tft.setCursor(15, 120);
  tft.println(" ");
  tft.setCursor(15, 145);
  tft.println("1");
  tft.setCursor(15, 170);
  tft.println("0");

  tft.setCursor(290, 8);
  tft.println("C");
  tft.setCursor(290, 30);
  tft.println("A");
  tft.setCursor(290, 52);
  tft.println("R");
  tft.setCursor(290, 74);
  tft.println("P");
  tft.setCursor(290, 96);
  tft.println("E");
  tft.setCursor(290, 118);
  tft.println(" ");
  tft.setCursor(290, 140);
  tft.println("C");
  tft.setCursor(290, 162);
  tft.println("A");
  tft.setCursor(290, 184);
  tft.println("R");
  tft.setCursor(290, 206);
  tft.println("P");

}





