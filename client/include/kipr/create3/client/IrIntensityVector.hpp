#pragma once

#include <vector>
#include "IrIntensity.hpp"

namespace kipr
{
  namespace create3
  {
    namespace client
    {
      class Create3IrIntensityVector : public std::vector<IrIntensity>
      {
        public:
      };
      using IrIntensityVector = Create3IrIntensityVector;
    }
  }
}