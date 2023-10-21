#include "kipr/create3/client/euler.h"

#include <cmath>

Create3Euler create3_euler_from_quaternion(const Create3Quaternion quaternion)
{
  const double q0 = quaternion.w;
  const double q1 = quaternion.x;
  const double q2 = quaternion.y;
  const double q3 = quaternion.z;

  Create3Euler euler;
  euler.x = std::atan2(2.0 * (q0 * q1 + q2 * q3), 1.0 - 2.0 * (q1 * q1 + q2 * q2));
  euler.y = std::asin(2.0 * (q0 * q2 - q3 * q1));
  euler.z = std::atan2(2.0 * (q0 * q3 + q1 * q2), 1.0 - 2.0 * (q2 * q2 + q3 * q3));

  return euler;
}
