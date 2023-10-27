#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  Create3DirectionForward = 1,
  Create3DirectionBackward = -1,
  Create3DirectionInvalid,
} Create3Direction;

Create3Direction create3_direction_from_string(const char* str);

#ifdef __cplusplus
}
#endif