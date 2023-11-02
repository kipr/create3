#pragma once

#include <vector>
#include "HazardDetection.hpp"

namespace kipr
{
  namespace create3
  {
    namespace client
    {
      class Create3HazardDetectionVector : public std::vector<HazardDetection>
      {
        public:
      };
      using HazardDetectionVector = Create3HazardDetectionVector;
    }
  }
}