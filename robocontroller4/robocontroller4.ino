#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
//#include <Servo.h>
#include <SparkFunAutoDriver.h>
#include <SparkFundSPINConstants.h>
#include <VarSpeedServo.h>

////inputs and output pins////
#define homeflagpin 29
#define headPin 6
#define heightPin 5

////Stepper Control Variables////
#define xCSPin 45
#define xBusyPin 46
#define xRSTPin 47
#define xAcc 600
#define xDec 1000
#define xVelMin 50
#define xVelMax 550
#define holdtorquepower 8
#define xPosMin 1
#define xPosMax 650

////Servo control variables////
#define headPosMin 10
#define headPosMax 245
#define headVel 50

#define heightPosMin 10
#define heightPosMax 245
#define heightVel 50


//somethings for later
#define commlength 3

//create servos
VarSpeedServo HeadAngle;  //SERVO FOR HEAD ANGLE
VarSpeedServo Height;      //servo FOR HEIGHT

//Autodriver Instances
AutoDriver XStepper(xCSPin, xBusyPin, xRSTPin);

//init position variables
int targetpos[] = {0, 0, 0};  //XPOS IN STEPS, HEAD ANGLE, HEIGHT ACTUATOR
int currentpos[] = {0, 0, 0}; //STORE THE CURRENT POSITION HERE AFTER MOTOR IS NO LONGER BUSY
int reqpos[] = {0, 0, 0};
int deltapos[] = {0, 0, 0};

void setup() {
  //attach servos to ports
  HeadAngle.attach(headPin, headPosMin, headPosMax); // attaches the servo on pin 10 to the servo object
  Height.attach(heightPin, heightPosMin, heightPosMax);

  //start serial comm
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing!");  // prints hello with ending line break

  //autodrive config
  XStepper.configSyncPin(BUSY_PIN, 0);
  XStepper.configStepMode(STEP_FS);
  XStepper.setMaxSpeed(xVelMax);
  XStepper.setMinSpeed(xVelMin);
  XStepper.setAcc(xAcc);
  XStepper.setDec(xDec);
  XStepper.setOCThreshold(OC_2625mA);
  XStepper.setHoldKVAL(holdtorquepower);
  XStepper.setLoSpdOpt(true);
  XStepper.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  XStepper.setSlewRate(SR_530V_us);
  zeroX();
}

void loop() {

  getinputs();

  checkpos();

  if (XStepper.busyCheck() == 0) //If the stepper motor isn't busy
  {
    Height.write(targetpos[2], heightVel);
    HeadAngle.write(targetpos[1], headVel);              // tell servo to go to position in variable 'pos'
    XStepper.goTo(targetpos[0]);
    for (int n = 0; n < 3; n++)
    {
      currentpos[n] = targetpos[n];
      deltapos[n] = 0;
    }
  }
  else
  {
    Serial.println("Stepper busy, wait a moment.");
  }

}
int zeroX() //zeros the x axis of the robot with the micro switch
{
  pinMode(homeflagpin, INPUT);
  while ( digitalRead(homeflagpin) == 0 );
  {
    XStepper.run(REV, 100);
  }
  while ( digitalRead(homeflagpin) == 1 );
  {
    XStepper.run(FWD, 20);
  }
  XStepper.hardStop();
  Serial.println("Zeroed");
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

