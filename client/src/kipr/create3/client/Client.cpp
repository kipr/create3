#include "kipr/create3/client/Client.hpp"

#include <capnp/ez-rpc.h>
#include <kipr/create3/create3.capnp.h>
#include <optional>

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

void Client::setVelocity(const double linear_x, const double angular_z)
{
  auto request = impl_->create3_client_.setVelocityRequest();
  Twist::Builder twist = request.initTwist();
  twist.setLinearX(linear_x);
  twist.setAngularZ(angular_z);
  request.send();
}