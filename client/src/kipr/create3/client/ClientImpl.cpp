#include "kipr/create3/client/ClientImpl.hpp"

using namespace kipr::create3;
using namespace kipr::create3::client;


RemoteClientImpl::RemoteClientImpl(const std::string &host, const std::uint16_t port)
  : client(kj::StringPtr(host.data()), port)
  , ClientImpl(host, port)
  , create3_client(client.getMain<Create3>())
{
}

kj::WaitScope &RemoteClientImpl::waitScope()
{
  return client.getWaitScope();
}

Create3::Client &RemoteClientImpl::create3Client()
{
  return create3_client;
}

LocalClientImpl::LocalClientImpl(kj::Own<Create3::Server> &&server)
  : create3_client(std::move(server))
  , ClientImpl(host, port)
  , loop()
  , wait(loop)
{
}

kj::WaitScope &LocalClientImpl::waitScope()
{
  return wait;
}

Create3::Client &LocalClientImpl::create3Client()
{
  return create3_client;
}
