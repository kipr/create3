#include "kipr/create3/client/Client.hpp"
#include "kipr/create3/client/client.h"

#include <mutex>
#include <thread>
#include <iostream>
#include <cmath>

namespace kipr
{
namespace create3
{
namespace client
{
  static std::unique_ptr<Client> global_client;
  static std::mutex global_client_mut;
}
}
}

using namespace kipr::create3::client;

int create3_connect()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (global_client)
  {
    std::cerr << __func__ << ": already connected" << std::endl;
    return true;
  }

  global_client = std::make_unique<Client>();

  std::cout << __func__ << ": Waiting for the Create 3 to connect..." << std::endl;

  while (!global_client->isConnected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << __func__ << ": Create 3 connected!" << std::endl;

  return true;
}

int create3_connect_manual(const char *const host, const unsigned short port)
{
  std::lock_guard<std::mutex> lock(global_client_mut);

  if (global_client)
  {
    std::cerr << __func__ << ": already connected" << std::endl;
    return false;
  }

  global_client = std::make_unique<Client>(host, port);

  std::cout << __func__ << ": Waiting for the Create 3 to connect..." << std::endl;

  while (!global_client->isConnected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << __func__ << ": Create 3 connected!" << std::endl;

  return true;
}

int create3_is_connected()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    return false;
  }

  return global_client->isConnected();
}

void create3_wait()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->wait();
}

void create3_execute_next_command_immediately()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->executeNextCommandImmediately();
}

/**
 * A silly alias for `create3_execute_next_command_immediately()`.
 */
void create3_execute_next_command_post_haste()
{
  create3_execute_next_command_immediately();
}

void create3_velocity_set(const Create3Twist twist)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  global_client->setVelocity(twist);
}

void create3_velocity_set_components(const double linear_x, const double angular_z)
{
  Create3Twist twist;
  twist.linear_x = linear_x;
  twist.angular_z = angular_z;
  create3_velocity_set(twist);
}

Create3Odometry create3_odometry_get()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return {};
  }
  return global_client->getOdometry();
}

Create3Pose create3_pose_get()
{
  return create3_odometry_get().pose;
}

Create3Quaternion create3_orientation_get_quaternion()
{
  return create3_pose_get().orientation;
}

double create3_orientation_get_quaternion_x()
{
  return create3_orientation_get_quaternion().x;
}

double create3_orientation_get_quaternion_y()
{
  return create3_orientation_get_quaternion().y;
}

double create3_orientation_get_quaternion_z()
{
  return create3_orientation_get_quaternion().z;
}

double create3_orientation_get_quaternion_w()
{
  return create3_orientation_get_quaternion().w;
}

Create3Euler create3_orientation_get_euler()
{
  return create3_euler_from_quaternion(create3_orientation_get_quaternion());
}

double create3_orientation_get_euler_x()
{
  return create3_orientation_get_euler().x;
}

double create3_orientation_get_euler_y()
{
  return create3_orientation_get_euler().y;
}

double create3_orientation_get_euler_z()
{
  return create3_orientation_get_euler().z;
}

Create3Twist create3_velocity_get()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  return global_client->getOdometry().velocity;
}

double create3_velocity_get_linear_x()
{
  return create3_velocity_get().linear_x;
}

double create3_velocity_get_angular_z()
{
  return create3_velocity_get().angular_z;
}

void create3_audio_play(const Create3AudioNote *const notes, const unsigned count, const int overwrite)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  global_client->playAudio(notes, count, overwrite);
}

void create3_audio_overwrite(const Create3AudioNote *const notes, const unsigned count)
{
  create3_audio_play(notes, count, 1);
}

void create3_audio_append(const Create3AudioNote *const notes, const unsigned count)
{
  create3_audio_play(notes, count, 0);
}

void create3_dock()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->dock();
}

void create3_undock()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->undock();
}

void create3_drive_straight(const float distance, const float max_linear_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->driveStraight(distance, max_linear_speed);
}

void create3_drive_arc_degrees(const float radius, const float angle, const float max_linear_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  const Create3Direction direction = angle > 0.0 ? Create3Direction::Create3DirectionForward : Create3Direction::Create3DirectionBackward;
  const float angle_rad = angle * M_PI / 180.0;

  global_client->driveArc(direction, radius, angle_rad, max_linear_speed);
}

void create3_drive_arc_radians(const float radius, const float angle, const float max_linear_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  const Create3Direction direction = angle > 0.0 ? Create3Direction::Create3DirectionForward : Create3Direction::Create3DirectionBackward;

  global_client->driveArc(direction, radius, angle, max_linear_speed);
}

void create3_rotate_degrees(const float angle, const float max_angular_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  const float angle_rad = angle * M_PI / 180.0;
  const float max_angular_speed_rad = max_angular_speed * M_PI / 180.0;

  global_client->rotate(angle_rad, max_angular_speed_rad);
}

void create3_rotate_radians(const float angle, const float max_angular_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->rotate(angle, max_angular_speed);
}

void create3_navigate_to_pose(const Create3Pose pose, const float max_linear_speed, const float max_angular_speed, const int achieve_goal_heading)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->navigateTo(pose, max_linear_speed, max_angular_speed, achieve_goal_heading);
}

void create3_navigate_to_position(
  const double x,
  const double y,
  const float max_linear_speed,
  const float max_angular_speed
)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->navigateTo(x, y, max_linear_speed, max_angular_speed);
}

void create3_navigate_to_position_with_heading(
  const double x,
  const double y,
  const double theta,
  const float max_linear_speed,
  const float max_angular_speed
)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->navigateTo(x, y, theta, max_linear_speed, max_angular_speed);
}

void create3_follow_wall(const Create3Follow follow, const float max_seconds)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->followWall(follow, max_seconds);
}