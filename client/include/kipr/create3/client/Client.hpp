#pragma once

#include <cstdint>
#include <string_view>
#include <memory>
#include <mutex>
#include <chrono>
#include <optional>

#include "AudioNote.hpp"
#include "Odometry.hpp"
#include "Twist.hpp"
#include "Follow.hpp"
#include "Pose.hpp"
#include "Direction.hpp"
#include "LedAnimationType.hpp"
#include "LedColor.hpp"
#include "Lightring.hpp"
#include "Duration.hpp"

namespace kipr
{
namespace create3
{
namespace client
{
  struct ClientImpl;

  template<typename T>
  struct Stamped
  {
    std::chrono::milliseconds at;
    T value;
  };

  class Client
  {
  public:
    Client(std::unique_ptr<ClientImpl> &&impl);
    Client(Client &&rhs) = default;
    Client(const std::string &host = "localhost", const std::uint16_t port = 50051);
    ~Client();

    bool isConnected();
    bool isDocked();

    void wait();
    void executeNextCommandImmediately();

    void setVelocity(const Twist &velocity);
    void playAudio(const AudioNote *const notes, const std::size_t count, const bool overwrite = false);

    void dock();
    void undock();

    void driveDistance(const float distance, const float max_linear_speed);
    void driveArc(const Direction direction, const float radius, const float angle, const float max_linear_speed);
    void ledAnimation(const LedAnimationType animation_type, const Lightring lightring, Duration duration);
    void lightring(const Lightring lightring);
    void rotate(const float angle, const float max_angular_speed);
    void navigateTo(const Pose &pose, const float max_linear_speed, const float max_angular_speed, const bool achieve_goal_heading = true);
    void navigateTo(const double x, const double y, const double theta, const float max_linear_speed, const float max_angular_speed);
    void navigateTo(const double x, const double y, const float max_linear_speed, const float max_angular_speed);
    void followWall(const Follow follow, const float max_seconds);


    Odometry getOdometry() const;

  private:
    Client(const Client &) = delete;

    std::unique_ptr<ClientImpl> impl_;

    mutable std::mutex wait_mut_;

    mutable std::mutex latest_odometry_mut_;
    mutable std::optional<Stamped<Odometry>> latest_odometry_;
  };
}
}
}