#include <gtest/gtest.h>

#include "kipr/create3/client/Client.hpp"
#include "kipr/create3/create3.capnp.h"
#include "kipr/create3/client/test_utils.hpp"

using namespace kipr::create3::client;

// Demonstrate some basic assertions.
TEST(Client, setVelocity) {  
  class Server final : public Create3::Server {
  public:
    Server(Twist::Reader &twist)
      : twist_(twist)
    {
    }

    kj::Promise<void> isConnected(IsConnectedContext context) override
    {
      context.initResults().setConnected(true);
    
      return kj::READY_NOW;
    }

    kj::Promise<void> setVelocity(SetVelocityContext context) override
    {
      twist_ = context.getParams().getTwist();

      return kj::READY_NOW;
    }

  private:
    Twist::Reader &twist_;
  };

  Twist::Reader twist;
  
  Client client(std::move(mockImpl(kj::heap<Server>(twist))));

  // client.setVelocity(1.0, 2.0);

  EXPECT_EQ(twist.getLinearX(), 1.0);
  EXPECT_EQ(twist.getAngularZ(), 2.0);
}