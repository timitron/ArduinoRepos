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

/////////////////////////////////////////////
////inputs and output pins////
/////////////////////////////////////////////
#define homeflagpin 42
#define headPin 6
#define heightPin 5

/////////////////////////////////////////////
////Stepper Control Variables////
/////////////////////////////////////////////
#define xCSPin 45
#define xBusyPin 46
#define xRSTPin 47
#define xAcc 600
#define xDec 1000
#define xVelMin 50
#define xVelMax 550
#define holdtorquepower 8
#define xPosMin 1
#define xPosMax 725

//Autodriver Instances
AutoDriver XStepper(xCSPin, xBusyPin, xRSTPin);

/////////////////////////////////////////////
////Servo control variables////
/////////////////////////////////////////////
#define headPosMin 10
#define headPosMax 245
#define headVel 50

#define heightPosMin 10
#define heightPosMax 245
#define heightVel 50

//create servos
VarSpeedServo HeadAngle;  //SERVO FOR HEAD ANGLE
VarSpeedServo Height;      //servo FOR HEIGHT

/////////////////////////////////////////////
//LCD Touchscreen over pins and variables //
/////////////////////////////////////////////

// These are the four touchscreen analog pins
#define YP A15  // must be an analog pin, use "An" notation!
#define XM A12  // must be an analog pin, use "An" notation!
#define YM A14   // can be a digital pin
#define XP A13   // can be a digital pin

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
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //last variable is resistance between X+ and X- pins.

//somethings for later
#define commlength 3

//init position variables
int targetpos[] = {0, 0, 0};  //XPOS IN STEPS, HEAD ANGLE, HEIGHT ACTUATOR
int currentpos[] = {0, 0, 0}; //STORE THE CURRENT POSITION HERE AFTER MOTOR IS NO LONGER BUSY
int reqpos[] = {0, 0, 0};
int deltapos[] = {0, 0, 0};

void setup() {
  //activate tft screen
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);          //blackscreen
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // White on black
  tft.setTextWrap(true); // Don't wrap text to next line
  tft.setTextSize(1); // large letters
  tft.setRotation(1); // horizontal display
  tft.println("TFT Activated");

  //attach servos to ports
  HeadAngle.attach(headPin, headPosMin, headPosMax); // attaches the servo on pin 10 to the servo object
  Height.attach(heightPin, heightPosMin, heightPosMax);
  tft.println("Servos attached");

  //start serial comm
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing!");  // prints hello with ending line break
  tft.println("Serial begin");

  //autodrive config
  XStepper.configSyncPin(BUSY_PIN, 0);    //sets pin to be busy check
  XStepper.configStepMode(STEP_FS);       //Sets single step mode add "_2^x" to the end where x is a number between 1-5
  XStepper.setMaxSpeed(xVelMax);          //max speed in rpm
  XStepper.setMinSpeed(xVelMin);          //min speed in rpm
  XStepper.setAcc(xAcc);                  //sets accel in steps /min/min
  XStepper.setDec(xDec);                  //sets decel in steps /min/min
  XStepper.setOCThreshold(OC_2625mA);     //over current threshhold, do not change only allows certain mA values
  XStepper.setHoldKVAL(holdtorquepower);  //sets holding torque power to a % of max, might need to increase later
  XStepper.setLoSpdOpt(true);             //better control for low speed steps, might try this toggled on and off.
  XStepper.setPWMFreq(PWM_DIV_2, PWM_MUL_2);  //PWM Frequency dividers and multipliers to keep the whine down
  XStepper.setSlewRate(SR_530V_us);       //Sets rate of increase in voltage between phases during a step, higher means more torque at large RPM.
  tft.println("Stepper Activated");
  zeroX();                                //Resets the x position

}

void loop() {
  //
  //
  //
  //
  //  getinputs();                            //I wrote this function to parse 3 integers into the reqpos arrau
  //
  //  checkpos();                             //checks the requested input to ensure that the position exists and doesn't cause a conflict
  //tryagain:
  //  if (XStepper.busyCheck() == 0)          //If the stepper motor busy pin is low then move the motor.
  //  {
  //    Height.write(targetpos[2], heightVel);               // move servo to target position at height velocity defined above.
  //    HeadAngle.write(targetpos[1], headVel);              // move servo to target position at height velocity defined above.
  //    XStepper.goTo(targetpos[0]);                         //move stepper motor to the requested position. Stepper decides direction, accel and speed.
  //    for (int n = 0; n < 3; n++)
  //    {
  //      currentpos[n] = targetpos[n];                      //set the target position to the current posision post-move
  //      deltapos[n] = 0;                                   //reset the change in position array which tracks if something is changing or not.
  //    }
  //  }
  //  else
  //  {
  //    Serial.println("Stepper busy, wait a moment.");      //It says the stepper is busy but it will still move, it just won't tell you.
  //    delay(25);                                           //give the stepper some time to not be busy
  //    goto tryagain;
  //  }
  //
  //}
}
int zeroX() //zeros the x axis of the robot with the micro switch
{
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
  tft.println("Zeroed");
  XStepper.resetPos();
  XStepper.goTo(250);
}

int checkpos() //function to validate requested position move
{
  if (reqpos[0] < xPosMin or reqpos[0] > xPosMax)
  {
    Serial.println("X POS OUTSIDE BOUNDS");
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
    Serial.println("HEAD ANGLE OUT OF BOUNDS");
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
    Serial.println("HEAD ANGLE OUT OF BOUNDS");
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
    Serial.println("No movement requested");
  }
  for (int n = 0; n < 3; n++)
  {
    Serial.println(targetpos[n]);
  }
}

int getinputs() //buffer serial interface
{
  while ( (Serial.available() < 3))
  {
    delay (5);
  }
  for (int n = 0; n < 3; n++)
  {
    reqpos[n] = Serial.parseInt(); // Then: Get them.
  }
}

