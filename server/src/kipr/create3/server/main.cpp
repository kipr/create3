#include <stdlib.h>
#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <irobot_create_msgs/msg/wheel_vels.hpp>
#include <irobot_create_msgs/action/dock.hpp>
#include <irobot_create_msgs/action/undock.hpp>

#include <kipr/create3/create3.capnp.h>
#include <capnp/ez-rpc.h>
#include <kj/async.h>

using namespace std::chrono_literals;

template<typename T>
struct ActionPromiseAdapter
{
  ActionPromiseAdapter(kj::PromiseFulfiller<typename T::Result::SharedPtr> &fulfiller, const typename rclcpp_action::Client<T>::SharedPtr &client, const typename T::Goal &goal)
    : fulfiller_(fulfiller)
  {
    using namespace std::placeholders;

    typename rclcpp_action::Client<T>::SendGoalOptions options;
    options.goal_response_callback = std::bind(&ActionPromiseAdapter::onGoalResponse, this, _1);
    options.result_callback = std::bind(&ActionPromiseAdapter::onResult, this, _1);

    client->async_send_goal(goal, options);
  }

private:
  void onGoalResponse(typename rclcpp_action::ClientGoalHandle<T>::SharedPtr handle)
  {
    if (handle)
    {
      return;
    }

    fulfiller_.reject(kj::Exception(kj::Exception::Type::FAILED, __FILE__, __LINE__));
  }

  void onResult(const typename rclcpp_action::ClientGoalHandle<T>::WrappedResult &result)
  {
    switch (result.code)
    {
      case rclcpp_action::ResultCode::SUCCEEDED:
        fulfiller_.fulfill(typename T::Result::SharedPtr(result.result));
        break;
      default:
        fulfiller_.reject(kj::Exception(kj::Exception::Type::FAILED, __FILE__, __LINE__));
        break;
    }
  }

  kj::PromiseFulfiller<typename T::Result::SharedPtr> &fulfiller_;
};

template<typename T>
using AdaptedAction = std::function<kj::Promise<typename T::Result::SharedPtr>(typename T::Goal)>;

template<typename T>
AdaptedAction<T> adaptAction(const typename std::shared_ptr<rclcpp_action::Client<T>> &client)
{
  return [client](auto goal) {
    if (!client->wait_for_action_server(10s))
    {
      return kj::Promise<typename T::Result::SharedPtr>(kj::Exception(kj::Exception::Type::FAILED, __FILE__, __LINE__));
    }

    return kj::newAdaptedPromise<typename T::Result::SharedPtr, ActionPromiseAdapter<T>>(client, goal);
  };
}

namespace create_msg = irobot_create_msgs::msg;
namespace create_action = irobot_create_msgs::action;

rclcpp_action::Client<create_action::Dock>::WrappedResult dock;

class Create3Node : public rclcpp::Node
{
public:
  Create3Node()
    : Node("create3")
    , cmd_vel_pub_(create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10))
    , dock(adaptAction(rclcpp_action::create_client<create_action::Dock>(this, "dock")))
    , undock(adaptAction(rclcpp_action::create_client<create_action::Undock>(this, "undock")))
  {
  }

  kj::Promise<void> setVelocity(const geometry_msgs::msg::Twist &velocity)
  {
    cmd_vel_pub_->publish(velocity);
    return kj::READY_NOW;
  }

  const AdaptedAction<create_action::Dock> dock;
  const AdaptedAction<create_action::Undock> undock;

private:
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
};

class Create3Server : public kipr::create3::Create3::Server
{
public:
  Create3Server(const std::shared_ptr<Create3Node> &node)
    : node_(node)
  {
  }

  kj::Promise<void> setVelocity(SetVelocityContext context) override
  {
    auto params = context.getParams();
    auto velocity = params.getVelocity();

    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = velocity.getLinearX();
    cmd_vel.angular.z = velocity.getAngularZ();

    return node_->setVelocity(cmd_vel);
  }

  kj::Promise<void> dock(DockContext context) override
  {
    return node_->dock(create_action::Dock::Goal {}).ignoreResult();
  }

  kj::Promise<void> undock(UndockContext context) override
  {
    return node_->undock(create_action::Undock::Goal {}).ignoreResult();
  }

  kj::Promise<void> getOdometry(GetOdometryContext context) override
  {
    auto response = context.getResults();

    return kj::READY_NOW;
  }

private:
  std::shared_ptr<Create3Node> node_;
};


int main(int argc, char *const argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Create3Node>();
  auto server = capnp::EzRpcServer(kj::heap<Create3Server>(node), "*", 8374);

  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
  }

  rclcpp::shutdown();

  return EXIT_SUCCESS;
}