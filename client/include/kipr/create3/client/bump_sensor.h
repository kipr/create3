#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  Create3BumpSensorLeft = 0,
  Create3BumpSensorFrontLeft = 1,
  Create3BumpSensorFrontCenter = 2,
  Create3BumpSensorFrontRight = 3,
  Create3BumpSensorRight = 4,
  Create3BumpSensorInvalid,
} Create3BumpSensor;

/**
 * @brief Converts a string to a Create3BumpSensor.
 * 
 * @param sensor The sensor to check
 * @param sensor_string The string to compare.
 * 
 * @return The Create3BumpSensor represented by the string, or Create3BumpSensorInvalid if the string is invalid.
*/
int create3_bump_sensor_to_string(int sensor, const char* sensor_string);

#ifdef __cplusplus
}
#endif