#include "kipr/create3/client/Client.hpp"

#include <capnp/ez-rpc.h>
#include <kipr/create3/create3.capnp.h>
#include <optional>
#include <chrono>
#include <iostream>
#include <cmath>
#include <string>

#include "kipr/create3/client/ClientImpl.hpp"

using namespace kipr::create3::client;

Client::Client(std::unique_ptr<ClientImpl> &&impl)
  : impl_(std::move(impl))
{
}

Client::Client(const std::string &host, const std::uint16_t port)
  : impl_(std::make_unique<RemoteClientImpl>(host, port))
{
}

Client::~Client()
{
}

std::string Client::getHost() const
{
  std::string host = impl_->host;
  return host;
}

std::uint16_t Client::getPort() const
{
  std::uint16_t port = impl_->port;
  return port;
}

bool Client::isConnected()
{
  auto request = impl_->create3Client().isConnectedRequest();
  const auto response = request.send().wait(impl_->waitScope());
  return response.getConnected();
}

void Client::wait()
{
  std::lock_guard<std::mutex> lock(wait_mut_);

  if (!impl_->last_waitable)
  {
    return;
  }

  impl_->last_waitable->wait(impl_->waitScope());
  impl_->last_waitable = std::nullopt;
}

void Client::executeNextCommandImmediately()
{
  std::lock_guard<std::mutex> lock(wait_mut_);
  // Forget about the last waitable
  impl_->last_waitable = std::nullopt;
}

void Client::setVelocity(const Twist &velocity)
{
  wait();

  auto request = impl_->create3Client().setVelocityRequest();
  auto velocityBuilder = request.initVelocity();
  velocityBuilder.setLinearX(velocity.linear_x);
  velocityBuilder.setAngularZ(velocity.angular_z);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::ledAnimation(const LedAnimationType animation_type, const Lightring lightring, const Duration duration)
{
  wait();

  auto request = impl_->create3Client().ledAnimationRequest();
  request.setAnimationType(static_cast<int8_t>(animation_type));

  auto lightringBuilder = request.initLightring();

  auto led0Builder = lightringBuilder.initLed0();
  led0Builder.setR(lightring.led0.r);
  led0Builder.setG(lightring.led0.g);
  led0Builder.setB(lightring.led0.b);

  auto led1Builder = lightringBuilder.initLed1();
  led1Builder.setR(lightring.led1.r);
  led1Builder.setG(lightring.led1.g);
  led1Builder.setB(lightring.led1.b);

  auto led2Builder = lightringBuilder.initLed2();
  led2Builder.setR(lightring.led2.r);
  led2Builder.setG(lightring.led2.g);
  led2Builder.setB(lightring.led2.b);

  auto led3Builder = lightringBuilder.initLed3();
  led3Builder.setR(lightring.led3.r);
  led3Builder.setG(lightring.led3.g);
  led3Builder.setB(lightring.led3.b);

  auto led4Builder = lightringBuilder.initLed4();
  led4Builder.setR(lightring.led4.r);
  led4Builder.setG(lightring.led4.g);
  led4Builder.setB(lightring.led4.b);

  auto led5Builder = lightringBuilder.initLed5();
  led5Builder.setR(lightring.led5.r);
  led5Builder.setG(lightring.led5.g);
  led5Builder.setB(lightring.led5.b);

  auto durationBuilder = request.initDuration();
  durationBuilder.setSeconds(duration.seconds);
  durationBuilder.setNanoseconds(duration.nanoseconds);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::dock()
{
  wait();

  auto request = impl_->create3Client().dockRequest();

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::undock()
{
  wait();

  auto request = impl_->create3Client().undockRequest();

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::driveDistance(const float distance, const float max_linear_speed)
{
  wait();

  auto request = impl_->create3Client().driveDistanceRequest();
  request.setDistance(distance);
  request.setMaxLinearSpeed(max_linear_speed);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::driveArc(const Direction direction, const float radius, const float angle, const float max_linear_speed)
{
  wait();

  auto request = impl_->create3Client().driveArcRequest();
  request.setDirection(static_cast<int8_t>(direction));
  request.setRadius(radius);
  request.setAngle(angle);
  request.setMaxLinearSpeed(max_linear_speed);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::rotate(const float angle, const float max_angular_speed)
{
  wait();

  auto request = impl_->create3Client().rotateRequest();
  request.setAngle(angle);
  request.setMaxAngularSpeed(max_angular_speed);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::navigateTo(const Pose &pose, const float max_linear_speed, const float max_angular_speed, const bool achieve_goal_heading)
{
  wait();

  auto request = impl_->create3Client().navigateToRequest();

  auto poseBuilder = request.initPose();
  auto positionBuilder = poseBuilder.initPosition();
  positionBuilder.setX(pose.position.x);
  positionBuilder.setY(pose.position.y);
  positionBuilder.setZ(pose.position.z);

  auto orientationBuilder = poseBuilder.initOrientation();
  orientationBuilder.setX(pose.orientation.x);
  orientationBuilder.setY(pose.orientation.y);
  orientationBuilder.setZ(pose.orientation.z);
  orientationBuilder.setW(pose.orientation.w);

  request.setMaxLinearSpeed(max_linear_speed);
  request.setMaxAngularSpeed(max_angular_speed);
  request.setAchieveGoalHeading(achieve_goal_heading);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

void Client::navigateTo(const double x, const double y, const double theta, const float max_linear_speed, const float max_angular_speed)
{
  navigateTo(Pose {
    .position = { .x = x, .y = y, .z = 0.0 },
    .orientation = { .x = 0.0, .y = 0.0, .z = std::sin(theta / 2.0), .w = std::cos(theta / 2.0) }
  }, max_linear_speed, max_angular_speed, true);
}

void Client::navigateTo(const double x, const double y, const float max_linear_speed, const float max_angular_speed)
{
  navigateTo(Pose {
    .position = { .x = x, .y = y, .z = 0.0 },
    .orientation = { .x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0 }
  }, max_linear_speed, max_angular_speed, false);
}

void Client::followWall(const Follow follow, const float max_seconds)
{
  wait();

  auto request = impl_->create3Client().followWallRequest();
  request.setFollow(static_cast<int8_t>(follow));
  request.setMaxSeconds(max_seconds);

  std::lock_guard<std::mutex> lock(wait_mut_);
  impl_->last_waitable = request.send().ignoreResult();
}

IrIntensityVector Client::getCliffIntensityVector() const
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  {
    std::lock_guard<std::mutex> lock(latest_cliff_intensity_vector_mut_);
    if (latest_cliff_intensity_vector_ && latest_cliff_intensity_vector_->at > system_clock::now().time_since_epoch() - 10ms)
    {
      return latest_cliff_intensity_vector_->value;
    }
  }

  auto request = impl_->create3Client().getCliffIntensityVectorRequest();
  const auto response = request.send().wait(impl_->waitScope());

  const auto cliff_intensity_vector = response.getCliffIntensityVector();
  IrIntensityVector result;
  for(size_t i = 0; i < cliff_intensity_vector.size(); ++i)
  {
    const auto cliff_intensity = cliff_intensity_vector[i];
    IrIntensity cliff_intensity_;
    cliff_intensity_.frameId = cliff_intensity.getFrameId().cStr();
    cliff_intensity_.intensity = cliff_intensity.getIntensity();
    cliff_intensity_.timestamp = cliff_intensity.getTimestamp();
    result.push_back(cliff_intensity_);
  }

  return result;

}

HazardDetectionVector Client::getHazardDetectionVector() const
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  {
    std::lock_guard<std::mutex> lock(latest_hazard_detection_vector_mut_);
    if (latest_hazard_detection_vector_ && latest_hazard_detection_vector_->at > system_clock::now().time_since_epoch() - 10ms)
    {
      return latest_hazard_detection_vector_->value;
    }
  }

  auto request = impl_->create3Client().getHazardDetectionVectorRequest();
  const auto response = request.send().wait(impl_->waitScope());

  const auto hazard_detection_vector = response.getHazardDetectionVector();
  HazardDetectionVector result;
  if(hazard_detection_vector.size() == 0)
  {
    return result;
  }
  for(size_t i = 0; i < hazard_detection_vector.size(); ++i)
  {
    const auto hazard_detection = hazard_detection_vector[i];
    HazardDetection hazard_detection_;
    hazard_detection_.type = hazard_detection.getType();
    hazard_detection_.frameId = hazard_detection.getFrameId().cStr();
    hazard_detection_.timestamp = hazard_detection.getTimestamp();
    result.push_back(hazard_detection_);
  }

  return result;
}

IrIntensityVector Client::getIrIntensityVector() const
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  {
    std::lock_guard<std::mutex> lock(latest_ir_intensity_vector_mut_);
    if (latest_ir_intensity_vector_ && latest_ir_intensity_vector_->at > system_clock::now().time_since_epoch() - 10ms)
    {
      return latest_ir_intensity_vector_->value;
    }
  }

  auto request = impl_->create3Client().getIrIntensityVectorRequest();
  const auto response = request.send().wait(impl_->waitScope());

  const auto ir_intensity_vector = response.getIrIntensityVector();
  IrIntensityVector result;
  for(size_t i = 0; i < ir_intensity_vector.size(); ++i)
  {
    const auto ir_intensity = ir_intensity_vector[i];
    IrIntensity ir_intensity_;
    ir_intensity_.frameId = ir_intensity.getFrameId().cStr();
    ir_intensity_.intensity = ir_intensity.getIntensity();
    ir_intensity_.timestamp = ir_intensity.getTimestamp();
    result.push_back(ir_intensity_);
  }

  return result;
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
  

  auto request = impl_->create3Client().getOdometryRequest();
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
  latest_odometry_ = Stamped<Odometry>{ .at = duration_cast<milliseconds>(system_clock::now().time_since_epoch()), .value = odometry };
  return odometry;
}