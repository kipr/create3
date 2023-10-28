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

/**
 * @brief Converts a double to a Create3Duration.
 * 
 * @param seconds The number of seconds.
 * 
 * @return The Create3Duration represented by the double.
*/
Create3Duration create3_duration_from_double(double seconds);

#ifdef __cplusplus
}
#endif