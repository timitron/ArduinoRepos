/*
	springspring421custom.h
	Created by Tim Lykins Spring 2016.
	Released into the public domain.
*/

#include "Arduino.h"
#include "spring421custom.h"
spring421custom::homeflagpin(int pin)
{
  _homeflagpin = pin;
}


void spring421custom::zeroX() { //zeros the x axis of the robot with the micro switch

  pinMode(_homeflagpin, INPUT);
  while ( digitalRead(_homeflagpin) == 0 );
  {
    XStepper.run(REV, 100);
  }
  while ( digitalRead(_homeflagpin) == 1 );
  {
    XStepper.run(FWD, 20);
  }
  XStepper.hardStop();
  delay(100);
  tft.println("Zeroed");
  XStepper.resetPos();
	XStepper.goTo(250);
	waitforX();
}

void spring421custom::checkpos() { //function to validate requested position move takes position in reqpos[] and puts it into targetpos[]

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

void spring421custom::getinputs() { //buffer serial interface

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

void spring421custom::shakey(int x) {
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

void spring421custom::waitforX() {
  while (XStepper.busyCheck() == 1)
  {
    delay(10);
  }

}

void spring421custom::RoboMove(int x, int headA, int heightA) {
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

void spring421custom::fishing(int steps, int angle) {
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