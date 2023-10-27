#pragma once

#include "led_animation_type.h"
#include <exception>

namespace kipr
{
namespace create3
{
namespace client
{
  using LedAnimationType = Create3LedAnimationType;

}
}
}

class InvalidLedAnimationTypeException : public std::exception {
public:
  virtual const char* what() const throw()
  {
    return "Invalid LedAnimationType";
  }
};