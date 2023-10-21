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

void create3_dock();

void create3_undock();

void create3_drive_straight(const float distance, const float max_linear_speed);

void create3_drive_arc(const Create3Direction direction, const float radius, const float angle, const float max_linear_speed);
    
void create3_rotate(const float angle, const float max_angular_speed);

void create3_navigate_to(const Create3Pose pose, const float max_linear_speed, const float max_angular_speed, const bool achieve_goal_heading);

void create3_follow_wall(const Create3Follow follow, const float max_seconds);

#ifdef __cplusplus
}
#endif