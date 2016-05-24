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
#define startPin  //this isn't attached yet
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
#define FRAME_H 220

#define BUTTONHEADER_X FRAME_X
#define BUTTONHEADER_Y FRAME_Y
#define BUTTONHEADER_W (FRAME_W)
#define BUTTONHEADER_H (FRAME_H/5)

#define BUTTONSERIAL_X FRAME_X
#define BUTTONSERIAL_Y (FRAME_Y+((FRAME_H/5)*1))
#define BUTTONSERIAL_W (FRAME_W)
#define BUTTONSERIAL_H (FRAME_H/5)

#define BUTTONMANUAL_X FRAME_X
#define BUTTONMANUAL_Y (FRAME_Y+((FRAME_H/5)*2))
#define BUTTONMANUAL_W (FRAME_W)
#define BUTTONMANUAL_H (FRAME_H/5)

#define BUTTONCOMPETITION_X FRAME_X
#define BUTTONCOMPETITION_Y (FRAME_Y+((FRAME_H/5)*3))
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
int boardHeight = 90;    //this is the angle of the height servo at which the fishing rod just touches the top of the blue game board
int tableHeight = 160;   //this is the angle at which the robot should drop fish off
int fishdiff = 65;
int fishtimeout = 10;

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
  

  //SD.begin(SD_CS);

  //yield();
  //bmpDraw("splash.bmp", 0, 0);
  modeselect();
}
void loop() {

  // if the screen has changed display the original menu
  if (menuString != "")
  {
    menuString = "";
    modeselect();
  }

  // Retrieve a point
  TSPoint p = ts.getPoint();      // See if there's any  touch data for us
  int cancel;
  if ((p.z > MINPRESSURE) && (p.z < MAXPRESSURE))
  {
    // Scale using the calibration #'s
    // and rotate coordinate system
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    int y = tft.height() - p.x;
    int x = p.y;

    //If the serial button was pushed
    if ((x > BUTTONSERIAL_X) && (x < (BUTTONSERIAL_X + BUTTONSERIAL_W)))    {
      if ((y > BUTTONSERIAL_Y) && (y <= (BUTTONSERIAL_Y + BUTTONSERIAL_H)))      {
        do
        { // do something over and over again until someone hits cancel
          cancel = 0;
          if (menuString == "")
          {
            menuString = "Serial Fishing";
            subdisplay();
            tft.setCursor(0, 50);
            tft.setTextSize(1);
            tft.setTextColor(ILI9341_ORANGE);
            tft.print("Waiting on inputs");
            xx = tft.getCursorX();
            yy = tft.getCursorY();
            iscrn = 0;                                                 //avoid a memory overflow
            selected = 1;
          }
          while ( (Serial.available() < 1) and (cancel == 0))
          { //checking for cancel button press
            waiting(12, xx, yy);
            TSPoint pp = ts.getPoint();      // See if there's any  touch data for us
            if ((pp.z > MINPRESSURE) && (pp.z < MAXPRESSURE)) // if there was a press swas is cancel?
            {
              pp.x = map(pp.x, TS_MINY, TS_MAXY, 0, tft.height());
              pp.y = map(pp.y, TS_MINX, TS_MAXX, 0, tft.width());
              int y = tft.height() - pp.x;
              int x = pp.y;
              if ((x > 20) && (x < 300))
              {
                if ((y > 180) && (y <= 210))
                {
                  cancel = 1;
                }
              }
            }
          }
          checkscreen();
          if (cancel == 0)
          {
            recvWithStartEndMarkers();

            if (newData)
            {
              parseData();
              tft.print("Waiting for inputs...");
              xx = tft.getCursorX();
              yy = tft.getCursorY();
            }

          }
        } while (cancel == 0);
      }
    }

    //if manual button was pushed
    if ((x > BUTTONMANUAL_X) && (x < (BUTTONMANUAL_X + BUTTONMANUAL_W)))    {
      if ((y > BUTTONMANUAL_X) && (y <= (BUTTONMANUAL_Y + BUTTONMANUAL_H)))      {
        do
        { // do something over and over again until someone hits cancel
          cancel = 0;
          if (menuString == "")
          {
            menuString = "Manual Control";
            subdisplay();
            tft.setCursor(0, 50);
            tft.setTextSize(1);
            tft.setTextColor(ILI9341_ORANGE);
            tft.print("Waiting on inputs");

            iscrn = 0;                                                 //avoid a memory overflow
            selected = 2;
          }
          while ( (Serial.available() < 1) and (cancel == 0))
          {
            TSPoint pp = ts.getPoint();      // See if there's any  touch data for us
            if ((pp.z > MINPRESSURE) && (pp.z < MAXPRESSURE))
            {
              pp.x = map(pp.x, TS_MINY, TS_MAXY, 0, tft.height());
              pp.y = map(pp.y, TS_MINX, TS_MAXX, 0, tft.width());
              int y = tft.height() - pp.x;
              int x = pp.y;
              if ((x > 20) && (x < 300))
              {
                if ((y > 180) && (y <= 210))
                {
                  cancel = 1;
                }
              }
            }
          }
          tft.println();
          checkscreen();
          if (cancel == 0)
          {

          }
        } while (cancel == 0);

      }
    }

    if ((x > BUTTONCOMPETITION_X) && (x < (BUTTONCOMPETITION_X + BUTTONCOMPETITION_W)))    {
      if ((y > BUTTONCOMPETITION_Y) && (y <= (BUTTONCOMPETITION_Y + BUTTONCOMPETITION_H)))      {
        do
        { // do something over and over again until someone hits cancel
          cancel = 0;
          if (menuString == "")
          {
            menuString = "Competition Mode";
            subdisplay();
            tft.setCursor(0, 50);
            tft.setTextSize(1);
            tft.setTextColor(ILI9341_ORANGE);
            iscrn = 0;                                                 //avoid a memory overflow
            selected = 3;
            xx = tft.getCursorX();
            yy = tft.getCursorY();
          }
          while ( (Serial.available() < 1) and (cancel == 0))
          { //checking for cancel button press

            waiting(12, xx, yy);
            TSPoint pp = ts.getPoint();      // See if there's any  touch data for us
            if ((pp.z > MINPRESSURE) && (pp.z < MAXPRESSURE)) // if there was a press swas is cancel?
            {
              pp.x = map(pp.x, TS_MINY, TS_MAXY, 0, tft.height());
              pp.y = map(pp.y, TS_MINX, TS_MAXX, 0, tft.width());
              int y = tft.height() - pp.x;
              int x = pp.y;
              if ((x > 20) && (x < 300))
              {
                if ((y > 180) && (y <= 210))
                {
                  cancel = 1;
                }
              }
            }
          }
          if (cancel == 0)
          {
            recvWithStartEndMarkers();

            if (newData)
            {
              parseData();
            }

          }
        } while (cancel == 0);
      }
    }
    if ((x > BUTTONDEBUG_X) && (x < (BUTTONDEBUG_X + BUTTONDEBUG_W)))    {
      if ((y > BUTTONDEBUG_Y) && (y <= (BUTTONDEBUG_Y + BUTTONDEBUG_H)))      {
        do
        { // do something over and over again until someone hits cancel
          cancel = 0;
          if (menuString == "")
          {
            menuString = "Debug Output";
            subdisplay();
            tft.setCursor(0, 50);
            tft.setTextSize(1);
            tft.setTextColor(ILI9341_ORANGE);
            tft.print("Waiting on inputs");
            xx = tft.getCursorX();
            yy = tft.getCursorY();
            iscrn = 0;                                                 //avoid a memory overflow
            selected = 4;
          }
          while ( (Serial.available() < 1) and (cancel == 0))
          {
            waiting(12, xx, yy);
            TSPoint pp = ts.getPoint();      // See if there's any  touch data for us
            if ((pp.z > MINPRESSURE) && (pp.z < MAXPRESSURE))
            {
              pp.x = map(pp.x, TS_MINY, TS_MAXY, 0, tft.height());
              pp.y = map(pp.y, TS_MINX, TS_MAXX, 0, tft.width());
              int y = tft.height() - pp.x;
              int x = pp.y;
              if ((x > 20) && (x < 300))
              {
                if ((y > 180) && (y <= 210))
                {
                  cancel = 1;
                }
              }
            }
          }
          checkscreen();
          if (cancel == 0)
          {

            recvWithStartEndMarkers();

            if (newData)
            {
              parseData();
              tft.print("Waiting for inputs...");
              xx = tft.getCursorX();
              yy = tft.getCursorY();
            }


          }
        } while (cancel == 0);
      }
    }
  }
  menuString == "";
  selected = 0;


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
  if (selected == 4)
  {
    tft.println();
    tft.print("Checking inputs:");
    for (int i = 0; i < 3; i++)
    {
      tft.print(reqpos[i]);
      tft.print(", ");
    }

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

int getinputs() { //buffer serial interface
  tft.print("Waiting on inputs");
  int x = tft.getCursorX();
  int y = tft.getCursorY();
  iscrn = 0;                                                 //avoid a memory overflow
  while ( (Serial.available() < 3))
  {
    waiting(12, x, y);
  }
  tft.println();
  checkscreen();
  for (int n = 0; n < 3; n++)
  {
    reqpos[n] = Serial.parseInt(); // Then: Get them.
  }
  tft.print("parsed: ");
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
  if (selected != 3) {
    tft.print("Fishing");
  }


  RoboMove(steps, angle, (boardHeight - 5));
  delay(600);                                                              //settling time for the fishing rod

  int fishcount = 0;
  int temp;
  
  do
  {
    Height.write((boardHeight - 5), 250);
    HeadAngle.write((angle), headVel);
    delay(750);
    //fishing rod up and down
    Height.write((boardHeight + 30), 250);                                      // move servo to target position at height velocity defined above.
    delay(333);

    //fish pull
    Height.write(0, 250);                                      // move servo to target position at height velocity defined above.
    fishcount++;
    delay(1000);
    temp = digitalRead(fishPin);
  } while ((temp==LOW) && (fishcount < fishtimeout));

  //turn back
  RoboMove(25, 5, 5);                                       // move servo to target position at height velocity defined above.
  delay(250);
  shakey(4);
}

int checkscreen() {
  if (tft.getCursorY() > 180 ) {
    tft.fillScreen(ILI9341_BLACK);                                     //blackscreen
    subdisplay();


    iscrn = 0;                                                              //resets counter if inside waiting loop with display
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_ORANGE);
    tft.println("Screen Cleared...");
    tft.setCursor(0, 50);
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

  //serial button
  tft.fillRect(BUTTONSERIAL_X, BUTTONSERIAL_Y, BUTTONSERIAL_W, BUTTONSERIAL_H, ILI9341_ORANGE);
  tft.setCursor(BUTTONSERIAL_X + 4 , BUTTONSERIAL_Y + (BUTTONSERIAL_H / 2));
  tft.setTextColor(ILI9341_BLACK);
  tft.println("Serial Control");

  //manual button
  tft.fillRect(BUTTONMANUAL_X, BUTTONMANUAL_Y, BUTTONMANUAL_W, BUTTONMANUAL_H, ILI9341_WHITE);
  tft.setCursor(BUTTONMANUAL_X + 4 , BUTTONMANUAL_Y + (BUTTONMANUAL_H / 2));
  tft.println("Manual Control");

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

void subdisplay () {
  tft.fillScreen(ILI9341_BLACK);

  //title
  tft.setTextSize(3);
  tft.fillRect(0, 0, 320, 45, ILI9341_ORANGE);
  tft.setCursor(4, 15);
  tft.setTextColor(ILI9341_BLACK);
  tft.println(menuString);

  //cancel button
  tft.setTextSize(2);
  tft.fillRect(20, 200, 280, 30, ILI9341_GREEN);
  tft.setCursor(30, 210);
  tft.setTextColor(ILI9341_RED);
  tft.println("CANCEL");

  //team 10 display
}


void parseData()
{
  newData = false;
  if (debug) {
    Serial.println( receivedChars );
  }

  // HELLO
  // If the Arduino receives "HELLO" it sends "HELLO" back
  // This is used by the VB program to show it is connected
  if (strcmp(receivedChars, "HELLO")  == 0)
  {
    tft.println("HELLO");
    Serial.println("Hello");
  }

  if (strcmp(receivedChars, "START")  == 0)
  {
    tft.println("Started...");
    Serial.println("Started...");
  }


  if (receivedChars[0] == 'F'  )
  {
    tft.print(convertToNumber( 1 ));
    fishing(convertToNumber( 1 ), convertToNumber(4));

  }

  if (receivedChars[0] == 'M'  )
  {
    tft.print(convertToNumber( 1 ));
    RoboMove(convertToNumber( 1  ), convertToNumber( 4 ), convertToNumber( 7 ));
  }

  if (receivedChars[0] == 'Z'  )
  {
    zeroX();
  }

  if (receivedChars[0] == 'H'  )
  {
    tft.println();
    tft.print("Board Height Set: ");
    tft.print(convertToNumber( 1 ));
    boardHeight = convertToNumber( 1  );
    tft.print(" Table Height Set: ");
    tft.println(convertToNumber( 4 ));
    tableHeight = convertToNumber( 4 );
  }

  if (receivedChars[0] == 'J'  )
  {
    tft.println();
    tft.print("Fish Pressure Difference Set: ");
    tft.print(convertToNumber( 1 ));
    fishdiff = convertToNumber( 1  );
    tft.print(" Fishing attempt timeout Set: ");
    tft.println(convertToNumber( 4 ));
    tableHeight = convertToNumber( 4 );
  }
  if ((receivedChars[0] == 'C'))
  {
    if (digitalRead(24) == LOW) //put a check for the game start pin here
    {
      fishing(convertToNumber( 1 ), convertToNumber( 4 ));
      delay(50);
      Serial.println("Next Pos?");
    }
  }
}
void recvWithStartEndMarkers()
{

  // function recvWithStartEndMarkers by Robin2 of the Arduino forums
  // See  http://forum.arduino.cc/index.php?topic=288234.0

  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';

  char rc;

  if (Serial.available() > 0)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }

}



/*********************
  converts 3 ascii characters to a numeric value

  Global:
   Expects receivedChars[] to contain the ascii characters

  Local:
   startPos is the position of the first character


*/

int convertToNumber( byte startPos)
{
  unsigned int tmp = 0;
  tmp = (receivedChars[startPos] - 48) * 100;
  tmp = tmp + (receivedChars[startPos + 1] - 48) * 10;
  tmp = tmp + receivedChars[startPos + 2] - 48;
  return tmp;
}



void sendOK(int val)
{
  // The 3 command buttons wait for the OK signal
  Serial.print("OK"); Serial.println(val);
}



