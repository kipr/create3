#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Contains a string for the frameId, the timestamp, and the value of the intensity
typedef struct
{
  const char* frameId;
  uint64_t timestamp;
  int16_t intensity;
} Create3IrIntensity;

#ifdef __cplusplus
}
#endif