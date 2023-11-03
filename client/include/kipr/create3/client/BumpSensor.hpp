#pragma once

#include "bump_sensor.h"
#include <exception>

namespace kipr
{
  namespace create3
  {
    namespace client
    {
    using BumpSensor = Create3BumpSensor;
    }
  }
}

class InvalidBumpSensorException : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
      return "Invalid Bump Sensor";
    }
};