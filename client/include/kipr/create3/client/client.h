#pragma once

#include "audio_note.h"
#include "twist.h"
#include "odometry.h"
#include "euler.h"
#include "direction.h"
#include "follow.h"
#include "pose.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int create3_connect();

int create3_connect_manual(const char *const host, const unsigned short port);

int create3_is_connected();

void create3_wait();

void create3_execute_next_command_immediately();

/**
 * A silly alias for `create3_execute_next_command_immediately()`.
 */
void create3_execute_next_command_post_haste();

/**
 * @brief Set the velocity of the iRobot Create 3.
 * 
 * @param twist The twist to set.
 */
void create3_velocity_set(const Create3Twist twist);

/**
 * @brief Set the velocity of the iRobot Create 3.
 * @details This is equivalent to calling
 * ```
 * Create3Twist twist;
 * twist.linear_x = linear_x;
 * twist.angular_z = angular_z;
 * create3_velocity_set(twist);
 * ```
 * 
 * @param linear_x The linear velocity in meters per second.
 * @param angular_z The angular velocity in radians per second.
 * 
 * @example `create3_velocity_set(1.0, 2.0); // Set the velocity to 1 m/s forward and 2 rad/s counter-clockwise.`
 */
void create3_velocity_set_components(const double linear_x, const double angular_z);

/**
 * @brief Get the odometry of the iRobot Create 3.
 * 
 * @return The odometry.
 */
Create3Odometry create3_odometry_get();

/**
 * @brief Get the pose of the iRobot Create 3.
 * 
 * @return The pose.
 */
Create3Pose create3_pose_get();

/**
 * @brief Get the orientation (as a quaternion) of the iRobot Create 3.
 * 
 * @return The orientation as a quaternion.
 */
Create3Quaternion create3_orientation_get_quaternion();

/**
 * @brief Get the X component of the orientation (as a quaternion) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_quaternion().x`.
 */
double create3_orientation_get_quaternion_x();

/**
 * @brief Get the Y component of the orientation (as a quaternion) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_quaternion().y`.
 */
double create3_orientation_get_quaternion_y();

/**
 * @brief Get the Z component of the orientation (as a quaternion) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_quaternion().z`.
 */
double create3_orientation_get_quaternion_z();

/**
 * @brief Get the W component of the orientation (as a quaternion) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_quaternion().w`.
 */
double create3_orientation_get_quaternion_w();

/**
 * @brief Get the orientation (as Euler angles) of the iRobot Create 3.
 * 
 * @return The orientation as Euler angles.
 */
Create3Euler create3_orientation_get_euler();

/**
 * @brief Get the X component of the orientation (as Euler angles) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_euler().x`.
 */
double create3_orientation_get_euler_x();

/**
 * @brief Get the Y component of the orientation (as Euler angles) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_euler().y`.
 */
double create3_orientation_get_euler_y();

/**
 * @brief Get the Z component of the orientation (as Euler angles) of the iRobot Create 3.
 * @details This is equivalent to calling `create3_orientation_get_euler().z`.
 */
double create3_orientation_get_euler_z();

/**
 * @brief Get the velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_odometry_get().velocity`.
 * 
 * @return The velocity.
 */
Create3Twist create3_velocity_get();

/**
 * @brief Get the linear X velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_velocity_get().linear_x`.
 * 
 * @return The linear X velocity in meters per second.
 */
double create3_velocity_get_linear_x();

/**
 * @brief Get the angular Z velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_velocity_get().angular_z`.
 * 
 * @return The angular Z velocity in radians per second.
 */
double create3_velocity_get_angular_z();

/**
 * @brief Play audio on the iRobot Create 3.
 * 
 * @param notes The notes to play.
 * @param count The number of notes to play.
 * @param overwrite 1 to overwrite the current audio queue, 0 to append instead.
 */
void create3_audio_play(const Create3AudioNote *const notes, const unsigned count, const int overwrite);

/**
 * @brief Overwrite the current audio queue with the given audio.
 * @details This is equivalent to calling `create3_audio_play(notes, count, 1)`.
 * 
 * @param notes The notes to play.
 * @param count The number of notes to play.
 */
void create3_audio_overwrite(const Create3AudioNote *const notes, const unsigned count);

/**
 * @brief Append the given audio to the current audio queue.
 * @details This is equivalent to calling `create3_audio_play(notes, count, 0)`.
 * 
 * @param notes The notes to play.
 * @param count The number of notes to play.
 */
void create3_audio_append(const Create3AudioNote *const notes, const unsigned count);

/**
 * @brief Dock the iRobot Create 3 on the closest Home Base.
 * @details The iRobot Create 3 must be undocked to dock.
 * 
 * @example `create3_dock();`
 */
void create3_dock();

/**
 * @brief Undock the iRobot Create 3 from the Home Base.
 * @details The iRobot Create 3 must be docked to undock. When undocking the iRobot Create 3 will drive backwards and turn 180 degrees.
 * 
 * @example `create3_undock();`
 */
void create3_undock();

/**
 * @brief Drive the iRobot Create 3 in an arc.
 * @details The iRobot Create 3 will drive in an arc with the given radius and angle at the given maximum linear speed. A negative angle will drive the iRobot Create 3 clockwise and a positive angle will drive the iRobot Create 3 counter-clockwise.
 * 
 * @param radius The radius of the arc in meters.
 * @param angle The angle of the arc in degrees.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s.
 * 
 * @example `create3_drive_arc_degrees(0.5, 90, 1.0); // Drive in a 0.5 meter radius arc counter-clockwise for 90 degrees at 1 m/s.`
 */
void create3_drive_arc_degrees(
  const float radius,
  const float angle,
  const float max_linear_speed
);

/**
 * @brief Drive the iRobot Create 3 in an arc.
 * @details The iRobot Create 3 will drive in an arc with the given radius and angle at the given maximum linear speed. A negative angle will drive the iRobot Create 3 clockwise and a positive angle will drive the iRobot Create 3 counter-clockwise.
 * 
 * @param radius The radius of the arc in meters.
 * @param angle The angle of the arc in radians.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s.
 * 
 * @example `create3_drive_arc_radians(0.5, 1.57, 1.0); // Drive in a 0.5 meter radius arc counter-clockwise for PI/2 radians at 1 m/s.`
 */
void create3_drive_arc_radians(
  const float radius,
  const float angle,
  const float max_linear_speed
);

/**
 * @brief Drive the iRobot Create 3 in a straight line.
 * @details The iRobot Create 3 will drive in a straight line for the given distance at the given maximum linear speed. A negative distance will drive the iRobot Create 3 backwards and a positive distance will drive the iRobot Create 3 forwards.
 * 
 * @param distance The distance to drive in meters.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s.
 * 
 * @example `create3_drive_straight(1.0, 0.5); // Drive forward for 1 meter at 0.5 m/s.`
 * @example `create3_drive_straight(-1.0, 0.5); // Drive backward for 1 meter at 0.5 m/s.`
 */
void create3_drive_straight(
  const float distance, 
  const float max_linear_speed
);

/**
 * @brief Rotate the iRobot Create 3 in place.
 * @details The iRobot Create 3 will rotate in place for the given angle in degrees at the given maximum angular speed. A negative angle will rotate the iRobot Create 3 clockwise and a positive angle will rotate the iRobot Create 3 counter-clockwise.
 * 
 * @param angle The angle to rotate in degrees.
 * @param max_angular_speed The maximum angular speed in degrees per second. Max with no safety overrides is 151 deg/s.
 * 
 */  
void create3_rotate_degrees(
  const float angle,
  const float max_angular_speed
);

/**
 * @brief Rotate the iRobot Create 3 in place.
 * @details The iRobot Create 3 will rotate in place for the given angle in radians at the given maximum angular speed. A negative angle will rotate the iRobot Create 3 clockwise and a positive angle will rotate the iRobot Create 3 counter-clockwise.
 * 
 * @param angle The angle to rotate in radians.
 * @param max_angular_speed The maximum angular speed in radians per second. Max with no safety overrides is 2.64 rad/s.
 * 
 */
void create3_rotate_radians(
  const float angle,
  const float max_angular_speed
);

void create3_navigate_to_pose(
  const Create3Pose pose,
  const float max_linear_speed,
  const float max_angular_speed,
  const int achieve_goal_heading
);

void create3_navigate_to_position(
  const double x,
  const double y,
  const float max_linear_speed,
  const float max_angular_speed
);

void create3_navigate_to_position_with_heading(
  const double x,
  const double y,
  const double theta,
  const float max_linear_speed,
  const float max_angular_speed
);

void create3_follow_wall(const Create3Follow follow, const float max_seconds);

#ifdef __cplusplus
}
#endif