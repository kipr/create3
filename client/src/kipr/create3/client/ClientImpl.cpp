#include "kipr/create3/client/ClientImpl.hpp"

using namespace kipr::create3::client;

ClientImpl::ClientImpl(const std::string_view &host, const std::uint16_t port)
  : client_(std::make_optional<capnp::EzRpcClient>(kj::StringPtr(host.data(), host.data() + host.size()), port))
  , create3_client_(client_->getMain<Create3>())
  , loop_()
  , wait_(loop_)
{
}

ClientImpl::ClientImpl(kj::Own<Create3::Server> &&server)
  : client_(std::nullopt)
  , create3_client_(std::move(server))
  , loop_()
  , wait_(loop_)
{
}

kj::WaitScope &ClientImpl::waitScope()
{
  if (client_)
  {
    return client_->getWaitScope();
  }

  return wait_;
}
