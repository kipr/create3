#include "kipr/create3/client/BumpSensor.hpp"
#include "kipr/create3/client/client.h"
#include "kipr/create3/client/Client.hpp"
#include "kipr/create3/client/Duration.hpp"
#include "kipr/create3/client/euler.h"
#include "kipr/create3/client/HazardDetectionVector.hpp"
#include "kipr/create3/client/IrIntensityVector.hpp"
#include "kipr/create3/client/LedAnimationType.hpp"
#include "kipr/create3/client/LedColor.hpp"
#include "kipr/create3/client/Lightring.hpp"
#include "kipr/create3/client/Quaternion.hpp"
#include "kipr/create3/client/Vector3.hpp"
#include "kj/exception.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string.h>
#include <thread>

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

// void create3_audio_play(const Create3AudioNote *const notes, const unsigned count, const int overwrite)
// {
//   std::lock_guard<std::mutex> lock(global_client_mut);
//   global_client->playAudio(notes, count, overwrite);
// }

// void create3_audio_overwrite(const Create3AudioNote *const notes, const unsigned count)
// {
//   create3_audio_play(notes, count, 1);
// }

// void create3_audio_append(const Create3AudioNote *const notes, const unsigned count)
// {
//   create3_audio_play(notes, count, 0);
// }

void resetGlobalClient()
{
  if (!global_client)
  {
    return;
  }

  std::cerr << __func__ << ": already connected. Trying to reconnect" << std::endl;
  global_client = nullptr;
}

void createNewClient(const char *const host, const unsigned short port)
{
  global_client = std::make_unique<Client>(host, port);
  std::cout << __func__ << ": Waiting for the Create 3 to connect..." << std::endl;

  while (!global_client->isConnected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << __func__ << ": Create 3 connected!" << std::endl;
}

void renewClient() {
  std::string host = global_client->getHost();
  std::uint16_t port = global_client->getPort();

  resetGlobalClient();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  createNewClient(host.data(), port);
}

int create3_connect()
{
  return create3_connect_manual("localhost", 50051);
}

int create3_connect_manual(const char *const host, const unsigned short port)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  resetGlobalClient();
  createNewClient(host, port);
  return true;
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

void create3_drive_straight(const float distance, const float max_linear_speed)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  global_client->driveDistance(distance, max_linear_speed);
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

void create3_execute_next_command_post_haste()
{
  create3_execute_next_command_immediately();
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

int create3_is_connected()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    return false;
  }

  return global_client->isConnected();
}

void create3_led_animation(const Create3LedAnimationType animation_type, const Create3Lightring lightring, const double max_runtime)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  Duration duration = create3_duration_from_double(max_runtime);
  global_client->ledAnimation(animation_type, lightring, duration);
}

Create3LedColor create3_led_color(const int r, const int g, const int b) {
  Create3LedColor color;
  color.r = (uint8_t) r;
  color.g = (uint8_t) g;
  color.b = (uint8_t) b;
  return color;
}

Create3Lightring create3_lightring(
    const Create3LedColor led0,
    const Create3LedColor led1,
    const Create3LedColor led2,
    const Create3LedColor led3,
    const Create3LedColor led4,
    const Create3LedColor led5
) {
  Create3Lightring lightring;
  lightring.led0 = led0;
  lightring.led1 = led1;
  lightring.led2 = led2;
  lightring.led3 = led3;
  lightring.led4 = led4;
  lightring.led5 = led5;
  return lightring;
}

void create3_lightring_set(const Create3Lightring lightring) {
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client) {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }
  Duration duration = create3_duration_from_double(1.0);
  global_client->ledAnimation(LedAnimationType::Create3BlinkLights, lightring, duration);
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

Create3Pose create3_pose_get()
{
  return create3_odometry_get().pose;
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

int create3_sensor_bump(int sensor_id)
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return 0;
  }

  HazardDetectionVector hazardSensors = global_client->getHazardDetectionVector();

  size_t hazard_num = hazardSensors.size();
  if(hazard_num == 0)
    return 0;

  for(const HazardDetection& hazardSensor: hazardSensors) {
    if(hazardSensor.type != 1) {
      continue;
    }
    // std::cout << "hazardSensor.frameId: " << hazardSensor.frameId << std::endl;
    if(create3_bump_sensor_to_string(sensor_id, hazardSensor.frameId) == 1) {
      return 1;
    }
    else {
      continue;
    }
  }
  return 0;

  // size_t hazard_num = hazardSensors.size();
  // if(hazard_num == 0)
  //   return 0;
  // for(size_t i = 0; i < hazard_num; i++)
  // {
  //   hazardSensors[i].type;
  //   if(hazardSensors[i].type != 1)
  //     continue;
  //   else {
  //     switch (sensor_id) {
  //       case Create3BumpSensorLeft:
  //         return strcmp(hazardSensors[i].frameId , "bump_left") == 0;
  //       case Create3BumpSensorFrontLeft:
  //         return strcmp(hazardSensors[i].frameId, "bump_front_left") == 0;
  //       case Create3BumpSensorFrontRight:
  //         return strcmp(hazardSensors[i].frameId, "bump_front_right") == 0;
  //       case Create3BumpSensorRight:
  //         return strcmp(hazardSensors[i].frameId, "bump_right") == 0;
  //       default:
  //         return 0;
  //     }
  //   }
  // }
  // return 0;
}

int create3_sensor_cliff(int sensor_id) {
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client) {
    std::cerr << __func__ << ": not connected" << std::endl;
    return 0;
  }
  
  IrIntensityVector cliffSensors = global_client->getCliffIntensityVector();
  return static_cast<int>(cliffSensors[sensor_id].intensity);
}

int create3_sensor_ir(int sensor_id) {
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client) {
    std::cerr << __func__ << ": not connected" << std::endl;
    return 0;
  }
  
  IrIntensityVector irSensors = global_client->getIrIntensityVector();
  return static_cast<int>(irSensors[sensor_id].intensity);
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

Create3Twist create3_velocity_get()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  return global_client->getOdometry().velocity;
}

double create3_velocity_get_angular_z()
{
  return create3_velocity_get().angular_z;
}

double create3_velocity_get_linear_x()
{
  return create3_velocity_get().linear_x;
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

void create3_wait()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (!global_client)
  {
    std::cerr << __func__ << ": not connected" << std::endl;
    return;
  }

  try {
    global_client->wait();
  } catch (kj::Exception &e) {
    std::cerr << __func__ << ": Create3 failed to wait for a command" << std::endl;
    renewClient();
  }
}