#include "kipr/create3/client/Client.hpp"

#include <capnp/ez-rpc.h>
#include <kipr/create3/create3.capnp.h>
#include <optional>
#include <chrono>

#include "kipr/create3/client/ClientImpl.hpp"

using namespace kipr::create3::client;

Client::Client(std::unique_ptr<ClientImpl> &&impl)
  : impl_(std::move(impl))
{
}

Client::Client(const std::string_view &host, const std::uint16_t port)
  : impl_(std::make_unique<ClientImpl>(host, port))
{
}

Client::~Client()
{
}

bool Client::isConnected()
{
  auto request = impl_->create3_client_.isConnectedRequest();
  const auto response = request.send().wait(impl_->waitScope());
  return response.getConnected();
}

void Client::wait()
{
  std::lock_guard<std::mutex> lock(wait_mut_);

  if (!impl_->last_waitable_)
  {
    return;
  }

  impl_->last_waitable_->wait(impl_->waitScope());
  impl_->last_waitable_ = std::nullopt;
}

void Client::executeNextCommandImmediately()
{
  std::lock_guard<std::mutex> lock(wait_mut_);
  // Forget about the last waitable
  impl_->last_waitable_ = std::nullopt;
}

void Client::setVelocity(const Twist &velocity)
{
  wait();

  auto request = impl_->create3_client_.setVelocityRequest();
  auto velocityBuilder = request.initVelocity();
  velocityBuilder.setLinearX(velocity.linear_x);
  velocityBuilder.setAngularZ(velocity.angular_z);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable_ = request.send().ignoreResult();
}

void Client::playAudio(const AudioNote *const notes, const std::size_t count, const bool overwrite)
{
  wait();

  auto request = impl_->create3_client_.playAudioRequest();
  auto notesBuilder = request.initNotes(count);

  for (std::size_t i = 0; i < count; ++i)
  {
    notesBuilder[i].setFrequency(notes[i].frequency);
    notesBuilder[i].setSeconds(notes[i].seconds);
  }
  
  request.setOverwrite(overwrite);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable_ = request.send().ignoreResult();
}

Odometry Client::getOdometry() const
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  {
    std::lock_guard<std::mutex> lock(latest_odometry_mut_);
    // Odometry remains valid for 10 milliseconds
    if (latest_odometry_ && latest_odometry_->at > system_clock::now().time_since_epoch() - 10ms)
    {
      return latest_odometry_->value;
    }
  }
  

  auto request = impl_->create3_client_.getOdometryRequest();
  const auto response = request.send().wait(impl_->waitScope());

  const auto pose = response.getPose();


  const auto position = pose.getPosition();
  const auto orientation = pose.getOrientation();

  Odometry odometry;
  
  odometry.pose.position.x = position.getX();
  odometry.pose.position.y = position.getY();
  odometry.pose.position.z = position.getZ();
  
  odometry.pose.orientation.x = orientation.getX();
  odometry.pose.orientation.y = orientation.getY();
  odometry.pose.orientation.z = orientation.getZ();
  odometry.pose.orientation.w = orientation.getW();

  const auto velocity = response.getVelocity();

  odometry.velocity.linear_x = velocity.getLinearX();
  odometry.velocity.angular_z = velocity.getAngularZ();

  std::lock_guard<std::mutex> lock(latest_odometry_mut_);
  latest_odometry_ = Stamped<Odometry>{ .at = system_clock::now().time_since_epoch(), .value = odometry };
  return odometry;
}