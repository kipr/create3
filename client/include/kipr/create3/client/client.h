#pragma once

#include "audio_note.h"
#include "direction.h"
#include "euler.h"
#include "follow.h"
#include "led_animation_type.h"
#include "led_color.h"
#include "lightring.h"
#include "odometry.h"
#include "pose.h"
#include "stdlib.h"
#include "twist.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Append the given audio to the current audio queue.
 * @details This is equivalent to calling `create3_audio_play(notes, count, 0)`.
 * 
 * @param notes The notes to play.
 * @param count The number of notes to play.
 */
void create3_audio_append(const Create3AudioNote *const notes, const unsigned count);

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
 * @brief Connect to the iRobot Create 3.
 * @details This will connect to the iRobot Create 3 at if running on a Wombat with the Create connected to the Wombat. Alternatively this will also work if the server and client are on the same device 
 * @return 1 if successful, 0 otherwise.
*/
int create3_connect();

/**
 * @brief Connect to the iRobot Create 3.
 * @details This will connect to the iRobot Create 3 at the given host and port.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * 
 * @return 1 if successful, 0 otherwise.
 * 
 * @example `create3_connect_manual("192.168.125.1", 50051); // Connect to the iRobot Create 3 on a dev computer connected to a Wombat running the client and a Wombat running the server.`
 */
int create3_connect_manual(const char *const host, const unsigned short port);

/**
 * @brief Dock the iRobot Create 3 on the closest Home Base.
 * @details The iRobot Create 3 must be undocked to dock.
 * 
 * @example `create3_dock();`
 */
void create3_dock();

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
 * @brief Execute the next command immediately.
 * @details This will execute the next command immediately instead of waiting for the last command to finish executing.
 * 
 * @example 
 * ```
 * create3_drive_straight(1.0, 0.5);
 * create3_execute_next_command_immediately();
 * create3_drive_straight(-1.0, 0.5);
 * create3_wait();
 * ```
 */
void create3_execute_next_command_immediately();

/**
 * A silly alias for `create3_execute_next_command_immediately()`.
 */
void create3_execute_next_command_post_haste();

/**
 * @brief Follow the given wall for the given amount of time.
 * @details The iRobot Create 3 will follow the given wall for the given amount of time at the given maximum linear speed. The iRobot Create 3 will stop when the given amount of time has elapsed.
 * 
 * @param follow The wall to follow. Either `Create3FollowLeft` or `Create3FollowRight`.
 * @param max_seconds The maximum amount of time to follow the wall in seconds.
 * 
 * @example `create3_follow_wall(Create3FollowLeft, 5.0); // Follow the left wall for 5 seconds.`
 */
void create3_follow_wall(const Create3Follow follow, const float max_seconds);

/**
 * @brief Check if the iRobot Create 3 is connected.
 * 
 * @return 1 if connected, 0 otherwise.
 */
int create3_is_connected();

/**
 * @brief Run the given LED animation on the iRobot Create 3.
 * @details This will run the given LED animation type and light ring colors on the iRobot Create 3 for the given amount of time.
 * 
 * @param animation_type The LED animation to run.
 * @param light_ring The light ring to use for the LED animation.
 * @param max_runtime The maximum amount of time to run the LED animation in seconds.
 * 
 * @example 
 * ```
 * struct Create3LedColor green;
 * green.r = 0;
 * green.g = 255;
 * green.b = 0;
 * 
 * struct Create3LightRing lightring;
 * lightring.led0 = green;
 * lightring.led1 = green;
 * lightring.led2 = green;
 * lightring.led3 = green;
 * lightring.led4 = green;
 * lightring.led5 = green;
 * 
 * create3_led_animation(Create3BlinkLights, lightring, 5.0); // Blink the lights green for 5 seconds.
 * ```
 */
void create3_led_animation(const Create3LedAnimationType animation_type, const Create3Lightring lightring, const double max_runtime);

/**
 * @brief Create an LED color from the given RGB values.
 * 
 * @param r The red value.
 * @param g The green value.
 * @param b The blue value.
 * 
 * @return The LED color.
 */
Create3LedColor create3_led_color(const int r, const int g, const int b);

/**
 * @brief Create a light ring from the given LED colors.
 * 
 * @param led0 The LED color for LED 0.
 * @param led1 The LED color for LED 1.
 * @param led2 The LED color for LED 2.
 * @param led3 The LED color for LED 3.
 * @param led4 The LED color for LED 4.
 * @param led5 The LED color for LED 5.
 * 
 * @return The light ring.
 */
Create3Lightring create3_lightring(
  const Create3LedColor led0,
  const Create3LedColor led1,
  const Create3LedColor led2,
  const Create3LedColor led3,
  const Create3LedColor led4,
  const Create3LedColor led5
);

/**
 * @brief Set the light ring of the iRobot Create 3.
 * @details This will set the light ring of the iRobot Create 3 to the given light ring colors.
 * 
 * @param lightring The light ring colors to set.
 * 
 * @example 
 * ```
 * struct Create3LedColor green;
 * green.r = 0;
 * green.g = 255;
 * green.b = 0;
 * 
 * struct Create3LightRing lightring;
 * lightring.led0 = green;
 * lightring.led1 = green;
 * lightring.led2 = green;
 * lightring.led3 = green;
 * lightring.led4 = green;
 * lightring.led5 = green;
 * 
 * create3_lightring(lightring); // Set the light ring to green.
 * ```
 */
void create3_lightring_set(const Create3Lightring lightring);

/**
 * @brief Navigate the iRobot Create 3 to the given pose.
 * @details The iRobot Create 3 will navigate to the given pose at the given maximum linear and angular speeds. The iRobot Create 3 will stop when it reaches the pose. The iRobot Create 3 will rotate in place to achieve the goal heading if `achieve_goal_heading` is 1.
 * 
 * @param pose The pose to navigate to. This contains a Vector3 for position and a Quaternion for orientation.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s. Used to get to the vector position.
 * @param max_angular_speed The maximum angular speed in radians per second. Max with no safety overrides is 2.64 rad/s. Used to get to the quaternion orientation.
 * @param achieve_goal_heading 1 to rotate in place to achieve the goal heading, 0 otherwise.
 * 
 * @example 
 * ```
 * Create3Pose pose;
 * pose.position.x = 1.0;
 * pose.position.y = 1.0;
 * pose.position.z = 0.0;
 * pose.orientation.x = 0.0;
 * pose.orientation.y = 0.0;
 * pose.orientation.z = 0.0;
 * pose.orientation.w = 1.0;
 * create3_navigate_to_pose(pose, 0.5, 1.0, 1); // Navigate to the given pose at 0.5 m/s and 1 rad/s and rotate in place to achieve the goal heading.
 * ```
 */
void create3_navigate_to_pose(
  const Create3Pose pose,
  const float max_linear_speed,
  const float max_angular_speed,
  const int achieve_goal_heading
);

/**
 * @brief Navigate the iRobot Create 3 to the given position.
 * @details The iRobot Create 3 will navigate to the given position at the given maximum linear and angular speeds. The iRobot Create 3 will stop when it reaches the position.
 * 
 * @param x The X component of the position to navigate to in meters.
 * @param y The Y component of the position to navigate to in meters.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s.
 * @param max_angular_speed The maximum angular speed in radians per second. Max with no safety overrides is 2.64 rad/s.
 * 
 * @example `create3_navigate_to_position(1.0, 1.0, 0.5, 1.0); // Navigate to the given position at 0.5 m/s and 1 rad/s.`
 */
void create3_navigate_to_position(
  const double x,
  const double y,
  const float max_linear_speed,
  const float max_angular_speed
);

/**
 * @brief Navigate the iRobot Create 3 to the given position with the given heading.
 * @details The iRobot Create 3 will navigate to the given position at the given maximum linear and angular speeds. The iRobot Create 3 will stop when it reaches the position. The iRobot Create 3 will rotate in place to achieve the goal heading.
 * 
 * @param x The X component of the position to navigate to in meters.
 * @param y The Y component of the position to navigate to in meters.
 * @param theta The heading to achieve in radians.
 * @param max_linear_speed The maximum linear speed in meters per second. Max with no safety overrides is 0.306 m/s.
 * @param max_angular_speed The maximum angular speed in radians per second. Max with no safety overrides is 2.64 rad/s.
 * 
 * @example `create3_navigate_to_position_with_heading(1.0, 1.0, 0.0, 0.5, 1.0); // Navigate to the given position at 0.5 m/s and 1 rad/s and rotate in place to achieve the goal heading.`
 */
void create3_navigate_to_position_with_heading(
  const double x,
  const double y,
  const double theta,
  const float max_linear_speed,
  const float max_angular_speed
);

/**
 * @brief Get the odometry of the iRobot Create 3.
 * 
 * @return The odometry.
 */
Create3Odometry create3_odometry_get();

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
 * @brief Get the pose of the iRobot Create 3.
 * 
 * @return The pose.
 */
Create3Pose create3_pose_get();

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

int create3_sensor_bump(int sensor_id);

int create3_sensor_cliff(int sensor_id);

int create3_sensor_ir(int sensor_id);

/**
 * @brief Undock the iRobot Create 3 from the Home Base.
 * @details The iRobot Create 3 must be docked to undock. When undocking the iRobot Create 3 will drive backwards and turn 180 degrees.
 * 
 * @example `create3_undock();`
 */
void create3_undock();

/**
 * @brief Get the velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_odometry_get().velocity`.
 * 
 * @return The velocity.
 */
Create3Twist create3_velocity_get();

/**
 * @brief Get the angular Z velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_velocity_get().angular_z`.
 * 
 * @return The angular Z velocity in radians per second.
 */
double create3_velocity_get_angular_z();

/**
 * @brief Get the linear X velocity of the iRobot Create 3.
 * @details This is equivalent to calling `create3_velocity_get().linear_x`.
 * 
 * @return The linear X velocity in meters per second.
 */
double create3_velocity_get_linear_x();

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
 * @brief Wait for the last command to finish executing.
 * @details This will block until the last command has finished executing.
 * 
 * @example 
 * ```
 * create3_drive_straight(1.0, 0.5);
 * create3_wait();
 * ```
 */
void create3_wait();

#ifdef __cplusplus
}
#endif