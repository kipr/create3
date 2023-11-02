#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Contains a string for the frameId, the timestamp, and the value of the intensity
typedef struct
{
  uint8_t type;
  const char* frameId;
  uint64_t timestamp;
} Create3HazardDetection;

#ifdef __cplusplus
}
#endif