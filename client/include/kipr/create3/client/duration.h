#pragma once

#include <stdint.h>

#include "quaternion.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int32_t seconds;
  uint32_t nanoseconds;
} Create3Duration;

Create3Duration create3_duration_from_double(double seconds);

#ifdef __cplusplus
}
#endif