#include <TouchScreen.h>
#include <pin_magic.h>
#include <registers.h>
#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"
#include <SparkFunAutoDriver.h>
#include <SparkFundSPINConstants.h>
#include <VarSpeedServo.h>
#include <SPI.h>
#include "Adafruit_GFX.h" // Adafruit graphics

////////////////////////////////////////////////////
//////board game defines/////////
////////////////////////////////////////////////////
#define boardHeight 70    //this is the angle of the height servo at which the fishing rod just touches the top of the blue game board
#define boardAngle 100    //The angle the board is rotated off of zero
#define headAngle 94      //the angle of the head when fishing hook is centered on the board
#define tableHeight 130   //this is the angle at which the robot should drop fish off
#define unloadAngle 0     //this is the angle at which the head is moved to in order to unload a fish
#define xunloadPos  50    //the step position of the x carriage for unloading 

/////////////////////////////////////////////
////inputs and output pins////
/////////////////////////////////////////////
#define homeflagpin 48
#define headPin 10
#define heightPin 11
#define startPin  //this isn't attached yet

/////////////////////////////////////////////
////Stepper Control Variables////
/////////////////////////////////////////////
#define xCSPin 45                             //stepper driver chip select pin  
#define xBusyPin 46                           //stepper driver busypin      
#define xRSTPin 47                            //reset stepper driver chip pin
#define xAcc 600                              //acel in steps/sec/sec
#define xDec 1000                             //dec in steps/sec/sec
#define xVelMin 50                            //minimum steps /sec
#define xVelMax 550                           //max steps/sec
#define holdtorquepower 16                    //% of max torque for holding between movements (WARNING GENERATES HEAT!) 
#define xPosMin 1                             //minimum step position logically
#define xPosMax 725                           //maximum logical step position      

//Autodriver Instances
AutoDriver XStepper(xCSPin, xRSTPin, xBusyPin);       //create instance of stepper

/////////////////////////////////////////////
////Servo control variables////
/////////////////////////////////////////////
#define headPosMin 10                         //PWM minimum value for angle 0    (needed to remove shaking)
#define headPosMax 245                        //pwm maximum value for angle 180 (needed to remove shaking)  
#define headVel 75                            //velocity of head actuator 1-255

#define heightPosMin 10                       //PWM minimum value for angle 0    (needed to remove shaking)
#define heightPosMax 245                      //PWM MAXIMUM value for angle 0    (needed to remove shaking)
#define heightVel 100                         //velocity of HEIGHT actuator 1-255

//create servos
VarSpeedServo HeadAngle;                                              //SERVO FOR HEAD ANGLE
VarSpeedServo Height;                                                 //servo FOR HEIGHT

/////////////////////////////////////////////
//LCD Touchscreen over pins and variables //
/////////////////////////////////////////////

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

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);                   //last variable is resistance between X+ and X- pins.

//somethings for later
#define commlength 3

//init position variables
int targetpos[] = {0, 0, 0};                                         //XPOS IN STEPS, HEAD ANGLE, HEIGHT ACTUATOR
int currentpos[] = {0, 0, 0};                                       //STORE THE CURRENT POSITION HERE AFTER MOTOR IS NO LONGER BUSY
int reqpos[] = {0, 0, 0};
int deltapos[] = {0, 0, 0};

void setup() {
  //activate tft screen
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);                                     //blackscreen
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);                    // White on black
  tft.setTextWrap(true);                                             // Don't wrap text to next line
  tft.setTextSize(1);                                                // large letters
  tft.setRotation(1);                                                // horizontal display
  tft.println("TFT Activated");

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

}

void loop() {
  getinputs();                                              //I wrote this function to parse 3 integers into the reqpos array
  RoboMove(reqpos[0], reqpos[1], reqpos[2]);                                                        
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
  XStepper.goTo(250);
  while (XStepper.busyCheck() == 1)
  {
    delay(10);
  }
}

int checkpos() { //function to validate requested position move takes position in reqpos[] and puts it into targetpos[]

  if (reqpos[0] < xPosMin or reqpos[0] > xPosMax)
  {
    tft.println("X POS OUTSIDE BOUNDS");
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
    tft.println("HEAD ANGLE OUT OF BOUNDS");
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
    tft.println("HEAD ANGLE OUT OF BOUNDS");
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
    tft.println("No movement requested");
  }
  tft.print("Moving to: ");
  for (int n = 0; n < 3; n++)
  {
    tft.print(targetpos[n]);
    tft.print(", ");
  }
  tft.println();
}

int getinputs() { //buffer serial interface

  while ( (Serial.available() < 3))
  {
    delay (5);
  }
  for (int n = 0; n < 3; n++)
  {
    reqpos[n] = Serial.parseInt(); // Then: Get them.
  }
  tft.print("inputs parsed: ");
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
    tft.print("Shakey!");
  }                                                   //rinse
  tft.println();
  Height.write(5, 150);
}

int waitforX() {
  while (XStepper.busyCheck() == 1)
  {
    delay(10);
  }

}

int RoboMove(int x, int headA, int heightA) {
  reqpos[0] = x;
  reqpos[1] = headA;
  reqpos[2] = heightA;
  checkpos();                             //checks the requested input to ensure that the position exists and doesn't cause a conflict
tryagain:
  if (XStepper.busyCheck() == 0)          //If the stepper motor busy pin is low then move the motor.
  {
    Height.write(targetpos[2], heightVel);               // move servo to target position at height velocity defined above.
    HeadAngle.write(targetpos[1], headVel);              // move servo to target position at height velocity defined above.
    XStepper.goTo(targetpos[0]);                         //move stepper motor to the requested position. Stepper decides direction, accel and speed.
    for (int n = 0; n < 3; n++)
    {
      currentpos[n] = targetpos[n];                      //set the target position to the current posision post-move
      deltapos[n] = 0;                                   //reset the change in position array which tracks if something is changing or not.
    }
  }
  else
  {
    tft.println("Stepper busy, wait a moment.");      //It says the stepper is busy but it will still move, it just won't tell you.
    delay(25);                                           //give the stepper some time to not be busy
    goto tryagain;
  }
  delay(1000);
}

int fishing(int steps, int angle) {
//  RoboMove(300, 90, 0);
//  delay(50);
    
  RoboMove(steps, angle, (boardHeight - 15));
  delay(1000);                                                              //settling time for the fishing rod

  //fishing rod up and down
  Height.write((boardHeight + 25), (heightVel * 2));                                      // move servo to target position at height velocity defined above.
  delay(100);

  //fish pull
  Height.write(5, 200);                                      // move servo to target position at height velocity defined above.
  HeadAngle.write((angle + 3), headVel);
  delay(50);

  //turn back
  RoboMove(25, 5, 5);                                       // move servo to target position at height velocity defined above.
  delay(25);
  shakey(3);
}


