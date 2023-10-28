#pragma once

#include <stdint.h>

#include "quaternion.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  double x;
  double y;
  double z;
} Create3Euler;

/**
 * @brief Converts a quaternion to Euler angles.
 * 
 * @param quaternion The quaternion to convert.
 * 
 * @return The Euler angles represented by the quaternion.
*/
Create3Euler create3_euler_from_quaternion(const Create3Quaternion quaternion);

/**
 * @brief Converts Euler angles to a quaternion.
 * 
 * @param euler The Euler angles to convert.
 * 
 * @return The quaternion represented by the Euler angles.
*/
Create3Quaternion create3_euler_to_quaternion(const Create3Euler euler);

#ifdef __cplusplus
}
#endif