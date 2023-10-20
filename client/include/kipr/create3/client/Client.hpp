#pragma once

#include <cstdint>
#include <string_view>
#include <memory>

namespace kipr
{
namespace create3
{
namespace client
{
  struct ClientImpl;

  class Client
  {
  public:
    Client(std::unique_ptr<ClientImpl> &&impl);
    Client(const std::string_view &host = "localhost", const std::uint16_t port = 8374);
    ~Client();

    bool isConnected();

    void setVelocity(const double linear_x, const double angular_z);

  private:
    std::unique_ptr<ClientImpl> impl_;
  };
}
}
}