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

/**
 * @brief Converts a string to a Create3Direction.
 * 
 * @param str The string to convert.
 * 
 * @return The Create3Direction represented by the string, or Create3DirectionInvalid if the string is invalid.
*/
Create3Direction create3_direction_from_string(const char* str);

#ifdef __cplusplus
}
#endif