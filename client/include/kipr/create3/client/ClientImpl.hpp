#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <variant>
#include <capnp/ez-rpc.h>
#include "kipr/create3/create3.capnp.h"

namespace kipr
{
namespace create3
{
namespace client
{
  struct ClientImpl
  {
    virtual kj::WaitScope &waitScope() = 0;
    virtual Create3::Client &create3Client() = 0;

    std::optional<kj::Promise<void>> last_waitable; 
  };

  struct RemoteClientImpl : public ClientImpl
  {
    RemoteClientImpl(const std::string &host, const std::uint16_t port);

    kj::WaitScope &waitScope() override;
    Create3::Client &create3Client() override;

    capnp::EzRpcClient client;
    Create3::Client create3_client;
  }; 

  struct LocalClientImpl : public ClientImpl
  {
    LocalClientImpl(kj::Own<Create3::Server> &&server);

    kj::WaitScope &waitScope() override;
    Create3::Client &create3Client() override;

    Create3::Client create3_client;
    kj::EventLoop loop;
    kj::WaitScope wait;
  };
}
}
}
