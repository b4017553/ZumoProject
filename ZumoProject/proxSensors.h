#pragma once

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ProximitySensors proxSensors;

const uint8_t proxThreshold = 6;

const uint16_t forwardSpeedMax = 100;

uint16_t forwardSpeedMin = 50;

uint16_t forwardSpeed = forwardSpeedMax;

const uint16_t deceleration = 2;

const uint16_t acceleration = 10;

// The time, in milliseconds, when an object was last seen.
uint16_t lastTimeObjectSeen = 0;

bool objectSeen = false;

bool objectInFront = false;

void proxSetup() {
  // put your setup code here, to run once:

  proxSensors.initThreeSensors();
  //Serial1.println();
  Serial1.println("Prox Sensors Initialised!");
  delay(2000);

}

void proxRead() {

  proxSensors.read();
  uint8_t frontLeftProx = proxSensors.countsFrontWithLeftLeds();
  uint8_t frontRightProx = proxSensors.countsFrontWithRightLeds();
  uint8_t leftLeftProx = proxSensors.countsLeftWithLeftLeds();
  uint8_t rightRightProx = proxSensors.countsRightWithRightLeds();

  // Serial1.print("Prox Sensors: ");

  // Serial1.print((String)leftLeftProx);
  // Serial1.print(" ");
  // Serial1.print(frontLeftProx);
  // Serial1.print(" ");
  // Serial1.print(frontRightProx);
  // Serial1.print(" ");
  // Serial1.print(rightRightProx);
  // Serial1.print("\n");

  objectSeen = frontLeftProx >= proxThreshold && frontRightProx >= proxThreshold;

  objectInFront = leftLeftProx >= proxThreshold || rightRightProx >= proxThreshold;

}

void moveToPerson()
{
  bool stopped = false;
  while(!stopped)
  {
    proxRead();
    uint8_t fLeftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t fRightValue = proxSensors.countsFrontWithRightLeds();
    uint8_t leftValue = proxSensors.countsLeftWithLeftLeds();
    uint8_t rightValue = proxSensors.countsRightWithRightLeds();

    objectSeen = fLeftValue >= proxThreshold || fRightValue >= proxThreshold;

    if (objectSeen)
    {
      // An object is visible, so we will start decelerating in
      // order to help the robot find the object without
      // overshooting
      Serial1.println("*Object Seen!");

      if(forwardSpeed > 0)
      {
        forwardSpeed -= deceleration;
      }
      //Serial1.println((String)forwardSpeed);    

      
    }
    else
    {
      // An object is not visible, so we will accelerate in order
      // to help find the object sooner.
      Serial1.println("Finding Object...");

      forwardSpeed +=acceleration;
    }

    forwardSpeed = constrain(forwardSpeed, forwardSpeedMin, forwardSpeedMax);

    if(objectInFront){

      Serial1.println("*Object In Front");

      ledYellow(1);

      lastTimeObjectSeen = millis();

      if(forwardSpeed > 50)
      {
        forwardSpeed = 50;
      }

      forwardSpeedMin = 0;
    }
    
    motors.setSpeeds(forwardSpeed, forwardSpeed);
        
    //Serial1.println((String)forwardSpeed);  

    if(forwardSpeed == 0)
    {
      stopped = true;
    }
  }
}
