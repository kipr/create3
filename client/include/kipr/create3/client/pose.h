#pragma once

#include "quaternion.h"
#include "vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  Create3Vector3 position;
  Create3Quaternion orientation;
} Create3Pose;

#ifdef __cplusplus
}
#endif