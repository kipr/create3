#include <gtest/gtest.h>

#include "kipr/create3/client/Client.hpp"
#include "kipr/create3/create3.capnp.h"
#include "kipr/create3/client/test_utils.hpp"

using namespace kipr::create3::client;

TEST(Client, setVelocity) {
  class Server final : public Create3::Server {
  public:
    Server(double &linear_x, double &angular_z)
      : linear_x_(linear_x)
      , angular_z_(angular_z)
    {
    }

    kj::Promise<void> setVelocity(SetVelocityContext context) override
    {
      auto twist = context.getParams().getTwist();
      linear_x_ = twist.getLinearX();
      angular_z_ = twist.getAngularZ();


      return kj::READY_NOW;
    }

  private:
    double &linear_x_;
    double &angular_z_;
  };

  double linear_x = 0.0;
  double angular_z = 0.0;
  
  mockClient(kj::heap<Server>(linear_x, angular_z)).setVelocity(1.0, 2.0);

  EXPECT_EQ(linear_x, 1.0);
  EXPECT_EQ(angular_z, 2.0);
}