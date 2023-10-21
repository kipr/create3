#pragma once

#include "pose.h"
#include "twist.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  Create3Pose pose;
  Create3Twist velocity;
} Create3Odometry;

#ifdef __cplusplus
}
#endif