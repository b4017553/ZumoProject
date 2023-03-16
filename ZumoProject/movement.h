#pragma once

#include <Wire.h>
#include <Zumo32U4.h>

#include "TurnSensor.h"
#include "proxSensors.h"
#include "encoders.h"

const uint8_t numSensors = 5;

uint16_t lineSensorValues[numSensors] = { 0, 0, 0, 0, 0};

// holds values of lines sensors on the left and right
uint16_t leftValues[numSensors];
uint16_t rightValues[numSensors];

//used to access lineSensorValues
uint8_t leftIndex = 0;
uint8_t middleIndex = 1;
uint8_t rightIndex = 2;

// may need changing - taken from borderdetect example
#define QTR_THRESHOLD   1000  // microseconds; 
#define WALL_THRESHOLD  100


// Motor speed when moving.
const uint16_t straightSpeed = 100;
// Motor speed when moving during line sensor calibration.
const uint16_t calibrationSpeed = 150;
// This line sensor threshold is used to detect walls.
const uint16_t sensorThreshold = 400;

const uint16_t wallThreshold = 1000;

// holds time i takes for robot to reach a wall
int timeToWall;
int16_t eToWall;

int averageTime;

// records position of walls
bool wallLeft = false;
bool wallRight = false;
bool wallFront = false;
bool halfWallRight = false;
bool halfWallLeft = false;

bool isjunction = false;

int32_t targetValue;

//Reads calibrated sensors to array
void readSensors()
{
  lineSensors.readCalibrated(lineSensorValues);
}

// resets targetValue to current angle as value 180 to -180
void targetValueReset()
{
  targetValue = returnAngle(turnAngle);
}

//Prints line sensor readings to Serial1 - Xbee
void printReadingsToSerial()
{
  char star = "*";
  static char buffer[80];
  sprintf(buffer, "%4d %4d %4d %4d %4d %4d \n",
    lineSensorValues[leftIndex],
    lineSensorValues[middleIndex],
    lineSensorValues[rightIndex],
    lineSensorValues[3],
    lineSensorValues[4],
    star
  );
  Serial1.print(buffer);
}


//moves forward
void moveForward()
{
    //Serial1.println("Moving forward...");
    motors.setSpeeds(forwardSpeed, forwardSpeed);
}

// stops the robots motors
void stop()
{
   motors.setSpeeds(0,0);
}

//checks if at intersection based on sensor threshold
bool atIntersection()
{

  readSensors();
  Serial1.println("*Intersection check...");
  printReadingsToSerial();

  if(lineSensorValues[middleIndex] > sensorThreshold || lineSensorValues[leftIndex] > sensorThreshold || lineSensorValues[rightIndex] > sensorThreshold)
  {
    return true;
  }  
  else if(lineSensorValues[leftIndex] > 20 && lineSensorValues[middleIndex] > 20 && lineSensorValues[rightIndex] > 20)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// turns to angle passed in
void turn(int32_t angle)
{

  Serial1.println("*Turning...");

  int ta  = returnAngle(turnAngle);
  int a = returnAngle(angle);

  //angle = angle - (int32_t)turnAngle1;

  if(a > ta || (a == -180 && ta > 0)){

    // turning left
    motors.setSpeeds(-turnSpeed,turnSpeed);

    if(a == -180)
    {
      while(ta != a)
      {
        //readSensors();
        //printReadingsToSerial();
        turnSensorUpdate();
        //Serial1.println(ta);
        ta  = returnAngle(turnAngle);
      }
    }
    else
    {
      while(ta < a)
      {
        turnSensorUpdate();
        ta  = returnAngle(turnAngle);
      }
    }
    stop();

  }
  if(a < ta){

    // turning right
    motors.setSpeeds(turnSpeed,-turnSpeed);

    while(ta > a)
    {
      turnSensorUpdate();
      ta  = returnAngle(turnAngle);
    }
    stop();

  }

}



void forwardUntil(int untilTime)
{
  // get position of Left line and Right line
  readSensors();
  printReadingsToSerial();

  int32_t targetAngle = targetValue;

  if(!atIntersection())
  {
    moveForward();
    wallFront = atIntersection();
  }
  else
  {
    wallFront = atIntersection();
  }

  int startTime = millis();

  int timePassed = millis() - startTime;

  while(timePassed < untilTime && !wallFront)
  {

    turnSensorUpdate();

    int32_t newAngle = (((int32_t)turnAngle >> 16) * 360) >> 16;

    int integral = 0.5;
    int Kp = 14;

    // calculates error based on difference between supposed angle and current
    int error = newAngle - targetAngle;

    if(error == 0){
      integral = 0;
    }
    else{
      integral = integral + error;
    }

    // used PID controller equations to work out correction needed to motors
    int correction = (Kp*error) + ((int)(error * integral));

    Serial1.println(correction);

    int leftSpeed = forwardSpeed + correction;

    int rightSpeed = forwardSpeed - correction;

    // constrains speeds so motors cant go any faster than 100
    leftSpeed = constrain(leftSpeed, 0,(int16_t)forwardSpeed);
    rightSpeed = constrain(rightSpeed, 0,(int16_t)forwardSpeed);

    motors.setSpeeds(leftSpeed,rightSpeed);

    Serial1.print(leftSpeed);
    Serial1.print(" ");
    Serial1.println(rightSpeed);

    readSensors();
    printReadingsToSerial();    

    timePassed = millis() - startTime;

    if(atIntersection())
    {
      stop();
      Serial1.println("*Reached Intersection");
      wallFront = true;    
      break;
    }
    else
    {
      wallFront = false;        
    }

  }

  stop();

}

// drives until reaches wall
void driveToLine(){

  int32_t saveAngle = targetValue;

  targetValueReset();

  int initTime = millis();

  encoderReset();

  wallFront = atIntersection();

  while(wallFront == false)
  {

    forwardUntil(forwardTime);

  }
  stop();
  int stopTime = millis();

  eToWall = getEncoderCount();

  timeToWall = stopTime - initTime;

  targetValue = saveAngle;

}

// sets motors to reverse
void reverse()
{
  Serial1.println("*Reversing...");
  motors.setSpeeds(-straightSpeed, -straightSpeed);
}

// reverses until off of a wall
void reverseUntil()
{
  encoderReset();

  if(atIntersection())
  {
    while(atIntersection())
    {
      reverse();
    }
    stop();
  }
}

// reverses for an amount of time passed in
void reverseFor(int time)
{
  int startTime = millis();
  int timePassed = millis() - startTime;

  while(timePassed < time)
  {
    reverse();

    timePassed = millis() - startTime;
  }

  stop();
}


//reverses until encoder count passed in
void reverseUntil(int16_t c)
{
  encoderReset();
  int16_t count = getEncoderCount();  

  while(count > -c)
  {
    reverse();
    count = getEncoderCount();
  }
  stop();
}

// prints line sensor values on the left and right
void printLineValues()
{
  // Serial1.print((String)leftValues[leftIndex]);
  // Serial1.print(" ");
  // Serial1.print(leftValues[middleIndex]);
  // Serial1.print(" ");
  // Serial1.print(leftValues[rightIndex]);
  // Serial1.print("\n");
  // Serial1.print(rightValues[leftIndex]);
  // Serial1.print(" ");
  // Serial1.print(rightValues[middleIndex]);
  // Serial1.print(" ");
  // Serial1.print(rightValues[rightIndex]);
  // Serial1.print("\n");
}


void checkForCorners()
{
  encoderReset();

  int32_t saveAngle = targetValue;

  targetValueReset();

  forwardUntil(timeToWall * 2.7);

  if(atIntersection())
  {
    isjunction = atIntersection();
    reverseUntil();
  }
  else
  {
    isjunction = atIntersection();
  }

  int16_t fCount = getEncoderCount();

  // turn right
  turn(turnAngle90);

  encoderReset();

  targetValueReset();

  // forward until wall or stop
  forwardUntil(timeToWall);

  int16_t rCount = getEncoderCount();

  // save right wall values
  lineSensors.readCalibrated(leftValues);

  reverseUntil(rCount);

  // turn to the left wall
  turn(-turnAngle90);

  encoderReset();

  targetValueReset();

  forwardUntil(timeToWall);

  int16_t lCount = getEncoderCount();

  lineSensors.readCalibrated(rightValues);

  reverseUntil(lCount);

  targetValue = saveAngle;

  turn(targetValue);

  if(currPosition != L_CORNER || currPosition != R_CORNER)
  {
    reverseUntil(fCount);
  }
  
}


// checks the walls that are in the robots current position
void checkForOpening()
{
  Serial1.println("*Checking for Opening...");

  bool saveWall = false;
  if(wallFront)
  {
    reverseUntil();
    saveWall = wallFront;
  }

  turn(turnAngle90);

  // Stop the motors.
  stop();

  driveToLine();

  //save values of left wall check
  lineSensors.readCalibrated(leftValues);

  reverseUntil();

  turn(-turnAngle90);

  driveToLine();

  // save values of right check
  lineSensors.readCalibrated(rightValues);
  
  reverseUntil();

  turn(targetValue);

  wallFront = saveWall;
  
  //Serial1.print("*Wall in Front: ");
  //Serial1.print(wallFront);

  printLineValues();
  
  // set walls
  if(leftValues[leftIndex] > 45 || leftValues[middleIndex] > 45 || leftValues[rightIndex] > 45)
  {
    wallLeft = true;
    Serial1.println("*Wall on Left");   
    if(leftValues[rightIndex] < 20)
    {
      Serial1.println("*Partial Left Wall");
      wallLeft = false;
    }
  }
  else
  {
    wallLeft = false;
  }
  if(rightValues[rightIndex] > 45 || rightValues[middleIndex] > 45 || rightValues[leftIndex] > 45)
  {
    wallRight = true;
    Serial1.println("*Wall on Right");

    if(rightValues[leftIndex] < 20)
    {
      Serial1.println("*Partial Right Wall");
      wallRight = false;
    }
  }
  else
  {
    wallRight = false;
  }


  //checking for half walls if no wall on one side

  if((wallLeft && !wallRight) || (!wallLeft && wallRight) || (!wallLeft && !wallRight))
  {
    checkForCorners();

    wallFront = isjunction;

    printLineValues();

    // set half walls
    if(leftValues[leftIndex] < 10 && leftValues[rightIndex] > 45)
    {
      Serial1.println("*half left wall");
      halfWallLeft = true;
    }
    else
    {
      Serial1.println("*No half left wall");
      halfWallLeft = false;
    }
    if(rightValues[leftIndex] > 45 && rightValues[rightIndex] < 10)
    {
      Serial1.println("*half right wall");
      halfWallRight = true;    
    }
    else
    {
      Serial1.println("*No half right wall");
      halfWallRight = false;
    }

    // just incase previous checks picked up and full wall and half a wall
    // stops false L and R corners.
    if(leftValues[leftIndex] < 20 && leftValues[rightIndex] < 20)
    {
      wallLeft = false;
    }
    if(rightValues[leftIndex] < 20 && rightValues[rightIndex] < 20)
    {
      wallRight = false;
    }    
  }
  else
  {
    halfWallLeft = false;
    halfWallRight = false;
  }

}

// used to straight the robots angle so it can drive straight
void fixAngle(char dir)
{
  encoderReset();

  driveToLine();

  eToWall = getEncoderCount();

  // reverse motors until both are off the line
  int16_t leftSpeed = 60;

  int16_t rightSpeed = 60;

  int16_t speedMax = 120;

  int16_t d = 10;


  motors.setSpeeds(-leftSpeed, -rightSpeed);

  readSensors();

  while(lineSensorValues[leftIndex] > 5 || lineSensorValues[rightIndex] > 5 )
  {

    if(lineSensorValues[leftIndex] > lineSensorValues[rightIndex])
    {
      rightSpeed = 0;
      leftSpeed += d;
    }
    else if(lineSensorValues[leftIndex] < lineSensorValues[rightIndex])
    {
      leftSpeed = 0;
      rightSpeed += d;
    }
    else
    {
      leftSpeed = 100;
      rightSpeed = 100;
    }

    leftSpeed = constrain(leftSpeed, 0,(int16_t)speedMax);
    rightSpeed = constrain(rightSpeed, 0,(int16_t)speedMax);

    motors.setSpeeds(-leftSpeed, -rightSpeed);

    readSensors();
    printReadingsToSerial();
  }

  stop();

  reverseUntil(50);

  targetValueReset();

  // set angle to 0
  turnSensorReset();

  if(dir == 'l')
  {
    turn(turnAngle90);
  }
  else if(dir == 'r')
  {
    turn(-turnAngle90);
  }

  turnSensorReset();

  targetValueReset();

  Serial1.println(targetValue);

  delay(2000);
}


void calibrateLineSensors()
{
  // To indicate we are in calibration mode, turn on the yellow LED
  // and print "Line cal" on the display.
  ledYellow(1);
  Serial1.println("Line calibration..");
  delay(2000);

  turnSensorReset();

  int ta = returnAngle(turnAngle);

  // Turn to the left 90 degrees.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while(ta < 90)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
    ta = returnAngle(turnAngle);
  }

  encoderReset();

  moveForward();

  int16_t tillCount = 1000;

  int16_t count = getEncoderCount();

  while(count < tillCount){
    lineSensors.calibrate();
    count = getEncoderCount();
  }

  stop();

  encoderReset();

  reverse();

  count = getEncoderCount();

  while(count > -tillCount){

    lineSensors.calibrate();
    count = getEncoderCount();
  }

  stop();

  // Turn to the right 90 degrees.
  motors.setSpeeds(calibrationSpeed, -calibrationSpeed);
  while(ta > -90)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
    ta = returnAngle(turnAngle);
  }

  stop();

  encoderReset();

  count = getEncoderCount();

  moveForward();

  while(count < tillCount){

    lineSensors.calibrate();
    count = getEncoderCount();
  }

  stop();

  encoderReset();

  count = getEncoderCount();

  reverse();

  while(count > -tillCount){

    lineSensors.calibrate();
    count = getEncoderCount();
  }

  stop();

  // Turn back to center using the gyro.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while(ta < 0)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
    ta = returnAngle(turnAngle);
  }

  // Stop the motors.
  stop();

  Serial1.println("Calibration Finished");
  delay(2000);
  ledYellow(0);
}


void movementSetup()
{
  // configures line sensor pins
  lineSensors.initThreeSensors();

  // calibrate gyro
  turnSensorSetup();

  // reset gyro to set current angle as zero
  turnSensorReset();

  // save current zero angle
  targetValueReset();

  // calibrate line sensors
  calibrateLineSensors();

}
