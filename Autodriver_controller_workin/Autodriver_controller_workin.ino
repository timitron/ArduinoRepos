#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Servo.h>
#include <SparkFunAutoDriver.h>
#include <SparkFundSPINConstants.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position
int vin = 0;

//Autodriver Instances
AutoDriver XStepper(48,49,47);
void setup() {
  // put your setup code here, to run once:
  //myservo.attach(10);  // attaches the servo on pin 10 to the servo object
  //serial comm
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing!");  // prints hello with ending line break
  
  //autodrive config

  XStepper.configSyncPin(BUSY_PIN,0);
  XStepper.configStepMode(STEP_FS_8);
  XStepper.setMaxSpeed(5);
  XStepper.setMinSpeed(1);
  XStepper.setAcc(2);
  XStepper.setDec(2);
  XStepper.setOCThreshold(OC_1500mA);
  XStepper.setHoldKVAL(32);
  

}

void loop() {
//  while (Serial.available() == 0);
//  pos = Serial.parseInt(); //read int or parseFloat for ..float...
//  Serial.println(pos);
// myservo.write(pos);              // tell servo to go to position in variable 'pos'

XStepper.run(FWD, 10);
XStepper.softStop();

}
