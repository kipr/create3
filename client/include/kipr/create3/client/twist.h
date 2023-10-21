#pragma once

#include "quaternion.h"
#include "vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  double linear_x;
  double angular_z;
} Create3Twist;

#ifdef __cplusplus
}
#endif