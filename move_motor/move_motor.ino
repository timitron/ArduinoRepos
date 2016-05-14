#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Servo.h>
#include <SparkFunAutoDriver.h>
#include <SparkFundSPINConstants.h>

//create servos
Servo HeadAngle;  //SERVO FOR HEAD ANGLE
Servo Height;      //servo FOR HEIGHT

//Autodriver Instances
AutoDriver XStepper(47, 49, 48);

//init variables
int targetpos[] = {0, 0, 0};  //XPOS IN STEPS, HEAD ANGLE, HEIGHT ACTUATOR
int currentpos[] = {0, 0, 0}; //STORE THE CURRENT POSITION HERE AFTER MOTOR IS NO LONGER BUSY
int deltapos[] = {0, 0, 0};
int pos = 0;

void setup() {
  //attach servos to ports
  HeadAngle.attach(6, 10, 245); // attaches the servo on pin 10 to the servo object
  Height.attach(5, 10, 245);

  //start serial comm
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing!");  // prints hello with ending line break

  //autodrive config
  XStepper.configSyncPin(BUSY_PIN, 0);
  XStepper.configStepMode(STEP_FS);
  XStepper.setMaxSpeed(500);
  XStepper.setMinSpeed(50);
  XStepper.setAcc(600);
  XStepper.setDec(1000);
  XStepper.setOCThreshold(OC_2625mA);
  XStepper.setHoldKVAL(8);
  XStepper.setLoSpdOpt(true);
  XStepper.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  XStepper.setSlewRate(SR_530V_us);

}

void loop() {
  for (int n = 0; n < 25; n++)
  {
    XStepper.goTo(650);
    while (XStepper.busyCheck() == 1) //If the stepper motor isn't busy
    {
      delay(15);
    }
    XStepper.goTo(0);
    while (XStepper.busyCheck() == 1) //If the stepper motor isn't busy
    {
      delay(15);
    }
  }
}
