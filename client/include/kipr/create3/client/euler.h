#pragma once

#include <stdint.h>

#include "quaternion.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  double x;
  double y;
  double z;
} Create3Euler;

Create3Euler create3_euler_from_quaternion(const Create3Quaternion quaternion);
Create3Quaternion create3_euler_to_quaternion(const Create3Euler euler);

#ifdef __cplusplus
}
#endif