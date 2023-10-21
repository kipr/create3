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

Create3Quaternion create3_euler_to_quaternion(const Create3Euler euler)
{
  const double c1 = std::cos(euler.x / 2.0);
  const double c2 = std::cos(euler.y / 2.0);
  const double c3 = std::cos(euler.z / 2.0);
  const double s1 = std::sin(euler.x / 2.0);
  const double s2 = std::sin(euler.y / 2.0);
  const double s3 = std::sin(euler.z / 2.0);

  Create3Quaternion quaternion;
  quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;
  quaternion.x = s1 * s2 * c3 + c1 * c2 * s3;
  quaternion.y = s1 * c2 * c3 + c1 * s2 * s3;
  quaternion.z = c1 * s2 * c3 - s1 * c2 * s3;

  return quaternion;
}