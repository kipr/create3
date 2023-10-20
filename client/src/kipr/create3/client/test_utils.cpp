#include "kipr/create3/client/test_utils.hpp"

using namespace kipr::create3::client;

std::unique_ptr<ClientImpl> &&kipr::create3::client::mockImpl(kj::Own<Create3::Server> &&server)
{
  return std::make_unique<ClientImpl>(std::move(server));
}