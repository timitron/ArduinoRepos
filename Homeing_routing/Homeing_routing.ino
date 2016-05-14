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
  pinMode(42, INPUT);
  while ( digitalRead(42) == 0 );
  {
    XStepper.run(REV, 100);
  }
  while ( digitalRead(42) == 1 );
  {
    XStepper.run(FWD, 20);
  }
  XStepper.hardStop();
  Serial.println("Zeroed");
  XStepper.resetPos();


  //  while ( (Serial.available() < 3))
  //  {
  //    // hang in this loop until we either get 9 bytes of data or 1 second
  //    // has gone by
  //  }
  //  for (int n = 0; n < 3; n++)
  //  {
  //    targetpos[n] = Serial.parseInt(); // Then: Get them.
  //    deltapos[n] = targetpos[n] - currentpos[n];
  //  }
  //  for (int n = 0; n < 3; n++)
  //  {
  //    Serial.println(targetpos[n]);
  //  }
  //  if (deltapos[0] == 0 and deltapos[1] == 0 and deltapos[2] == 0)
  //  {
  //    Serial.println("No movement requested");
  //  }
  //  else
  //  {
  //    if (XStepper.busyCheck() == 0) //If the stepper motor isn't busy
  //    {
  //      Height.write(targetpos[2]);
  //      HeadAngle.write(targetpos[1]);              // tell servo to go to position in variable 'pos'
  //      XStepper.goTo(targetpos[0]);
  //      for (int n = 0; n < 3; n++)
  //      {
  //        currentpos[n]=targetpos[n];
  //        deltapos[n]=0;
  //      }
  //    }
  //    else
  //    {
  //      Serial.println("Stepper busy, retry command later.");
  //    }
  //  }
  //  //  Serial.flush();
  //  //
  //  //  if (Serial.available() < 3)
  //  //  {
  //  //
  //  //    for (int i = 0; i < 3; i++)
  //  //    {
  //  //      targetpos[i] = Serial.parseInt(); //read int or parseFloat for ..float...;
  //  //      Serial.println(targetpos[i]);
  //  //      i++;
  //  //    }
  //  //
  //  //
  //  //
  //  //    if (targetpos[0] != currentpos[0] or targetpos[1] != currentpos[1] or targetpos[2] != currentpos[2]) //If the position changed
  //  //    {
  //  //      if (XStepper.busyCheck() == 0) //If the stepper motor isn't busy
  //  //        {
  //  //        XStepper.goTo(targetpos[0]);  //Move xtepper
  //  //        HeadAngle.write(targetpos[1]); //move head angle servo
  //  //        Height.write(targetpos[2]);     //move height servo
  //  //        //Now somde debug output
  //  //        Serial.write("Movement Accepted; Going to POS(");
  //  //        Serial.write(targetpos[0]);
  //  //        Serial.write(",");
  //  //        Serial.write(targetpos[1]);
  //  //        Serial.write(",");
  //  //        Serial.write(targetpos[2]);
  //  //        Serial.write(")");
  //  //        deltapos[0] = 0;
  //  //        deltapos[1] = 0;
  //  //        deltapos[2] = 0;
  //  //      }
  //  //      else
  //  //      {
  //  //        Serial.println("STEPPER BUSY: RETRY COMMAND"); //return error is stepper is busy
  //  //      }
  //  //    }
  //  //    else
  //  //    {
  //  //      Serial.println("No change in position requested"); //is the position is the same as the current position return a string
  //  //    }
  //  //  }
}
