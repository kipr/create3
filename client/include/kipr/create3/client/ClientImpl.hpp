#pragma once

#include <string_view>
#include <cstdint>
#include <optional>
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
    ClientImpl(const std::string_view &host, const std::uint16_t port);

    ClientImpl(kj::Own<Create3::Server> &&server);

    kj::WaitScope &waitScope();

    std::optional<capnp::EzRpcClient> client_;
    kj::EventLoop loop_;
    kj::WaitScope wait_;
    Create3::Client create3_client_;
  }; 
}
}
}
