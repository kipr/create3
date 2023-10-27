#pragma once

#include "direction.h"
#include <exception>

namespace kipr
{
namespace create3
{
namespace client
{
  using Direction = Create3Direction;
}
}
}

class InvalidDirectionException : public std::exception
{
  public:
    virtual const char* what() const throw()
    {
      return "Invalid Direction";
    }
};