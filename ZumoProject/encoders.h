#pragma once

#include <Wire.h>
#include <Zumo32U4.h>

void encoderReset()
{
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

int16_t getEncoderCount()
{
  return (encoders.getCountsLeft() + encoders.getCountsRight())/2;
}