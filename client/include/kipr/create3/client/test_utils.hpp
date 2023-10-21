#pragma once

#include <memory>
#include "ClientImpl.hpp"
#include "Client.hpp"
#include "kipr/create3/create3.capnp.h"

namespace kipr
{
namespace create3
{
namespace client
{
  std::unique_ptr<ClientImpl> mockImpl(kj::Own<Create3::Server> &&server);
  Client mockClient(kj::Own<Create3::Server> &&server);
}
}
}