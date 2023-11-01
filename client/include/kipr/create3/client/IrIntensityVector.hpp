#pragma once

#include <vector>
// #include "ir_intensity_vector.h"
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
          Create3IrIntensityVector()
          {
          }
          Create3IrIntensityVector(IrIntensity *vector, int size)
            : std::vector<IrIntensity>(vector, vector + size)
          {
          }
          Create3IrIntensityVector(const Create3IrIntensityVector &rhs)
            : std::vector<IrIntensity>(rhs)
          {
          }
          Create3IrIntensityVector(Create3IrIntensityVector &&rhs)
            : std::vector<IrIntensity>(std::move(rhs))
          {
          }
          Create3IrIntensityVector &operator=(const Create3IrIntensityVector &rhs)
          {
            std::vector<IrIntensity>::operator=(rhs);
            return *this;
          }
          Create3IrIntensityVector &operator=(Create3IrIntensityVector &&rhs)
          {
            std::vector<IrIntensity>::operator=(std::move(rhs));
            return *this;
          }
          ~Create3IrIntensityVector()
          {
          }
      };
      using IrIntensityVector = Create3IrIntensityVector;
    }
  }
}