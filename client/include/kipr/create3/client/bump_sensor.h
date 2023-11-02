#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  Create3BumpSensorLeft = 0,
  Create3BumpSensorFrontLeft = 1,
  Create3BumpSensorFrontRight = 2,
  Create3BumpSensorRight = 3,
  Create3BumpSensorInvalid,
} Create3BumpSensor;

/**
 * @brief Converts a string to a Create3BumpSensor.
 * 
 * @param str The string to convert.
 * 
 * @return The Create3BumpSensor represented by the string, or Create3BumpSensorInvalid if the string is invalid.
*/
Create3BumpSensor create3_bump_sensor_from_string(const char* str);

#ifdef __cplusplus
}
#endif