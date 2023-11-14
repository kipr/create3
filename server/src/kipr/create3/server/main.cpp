#include <stdlib.h>
#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include <irobot_create_msgs/action/audio_note_sequence.hpp>
#include <irobot_create_msgs/action/dock.hpp>
#include <irobot_create_msgs/action/drive_arc.hpp>
#include <irobot_create_msgs/action/drive_distance.hpp>
#include <irobot_create_msgs/action/led_animation.hpp>
#include <irobot_create_msgs/action/navigate_to_position.hpp>
#include <irobot_create_msgs/action/rotate_angle.hpp>
#include <irobot_create_msgs/action/undock.hpp>
#include <irobot_create_msgs/action/wall_follow.hpp>

#include <irobot_create_msgs/msg/hazard_detection_vector.hpp>
#include <irobot_create_msgs/msg/ir_intensity_vector.hpp>
#include <irobot_create_msgs/msg/led_color.hpp>
#include <irobot_create_msgs/msg/wheel_vels.hpp>

#include <kipr/create3/create3.capnp.h>
#include <capnp/ez-rpc.h>
#include <kj/async.h>

#include <iostream>

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

    fulfiller_.reject(kj::Exception(kj::Exception::Type::FAILED, __FILE__, __LINE__, kj::heapString("Failed to send goal")));
  }

  void onResult(const typename rclcpp_action::ClientGoalHandle<T>::WrappedResult &result)
  {
    switch (result.code)
    {
      case rclcpp_action::ResultCode::SUCCEEDED:
        fulfiller_.fulfill(typename T::Result::SharedPtr(result.result));
        break;
      default:
        std::ostringstream ss;
        ss << "Action failed with code " << static_cast<int>(result.code);
        fulfiller_.reject(kj::Exception(
          kj::Exception::Type::FAILED,
          __FILE__,
          __LINE__,
          kj::heapString(ss.str())
        ));
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
      return kj::Promise<typename T::Result::SharedPtr>(kj::Exception(
        kj::Exception::Type::FAILED,
        __FILE__,
        __LINE__,
        kj::heapString("Failed to connect to action server")
      ));
    }

    return kj::newAdaptedPromise<typename T::Result::SharedPtr, ActionPromiseAdapter<T>>(client, goal);
  };
}

namespace create_action = irobot_create_msgs::action;
namespace create_msg = irobot_create_msgs::msg;

// rclcpp_action::Client<create_action::Dock>::WrappedResult dock;

class Create3Node : public rclcpp::Node
{
public:
  Create3Node()
    : Node("create3")
    , dock(adaptAction(rclcpp_action::create_client<create_action::Dock>(this, "dock")))
    , drive_arc(adaptAction(rclcpp_action::create_client<create_action::DriveArc>(this, "drive_arc")))
    , drive_distance(adaptAction(rclcpp_action::create_client<create_action::DriveDistance>(this, "drive_distance")))
    , led_animation(adaptAction(rclcpp_action::create_client<create_action::LedAnimation>(this, "led_animation")))
    , navigate_to(adaptAction(rclcpp_action::create_client<create_action::NavigateToPosition>(this, "navigate_to_position")))
    , rotate(adaptAction(rclcpp_action::create_client<create_action::RotateAngle>(this, "rotate_angle")))
    , undock(adaptAction(rclcpp_action::create_client<create_action::Undock>(this, "undock")))

    , cmd_vel_pub_(create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10))

    , cliff_intensity_sub_(create_subscription<create_msg::IrIntensityVector>(
        "cliff_intensity",
        rclcpp::QoS(rclcpp::KeepLast(10), rmw_qos_profile_sensor_data),
        std::bind(&Create3Node::onCliffIntensity, this, std::placeholders::_1)
      ))

    , hazard_detection_sub_(create_subscription<create_msg::HazardDetectionVector>(
        "hazard_detection",
        rclcpp::QoS(rclcpp::KeepLast(10), rmw_qos_profile_sensor_data),
        std::bind(&Create3Node::onHazardDetection, this, std::placeholders::_1)
      ))

    , ir_intensity_sub_(create_subscription<create_msg::IrIntensityVector>(
        "ir_intensity",
        rclcpp::QoS(rclcpp::KeepLast(10), rmw_qos_profile_sensor_data),
        std::bind(&Create3Node::onIrIntensity, this, std::placeholders::_1)
      ))        
        
    , odom_sub_(create_subscription<nav_msgs::msg::Odometry>(
        "odom",
        rclcpp::QoS(rclcpp::KeepLast(10), rmw_qos_profile_sensor_data),
        std::bind(&Create3Node::onOdom, this, std::placeholders::_1)
      ))
  {
  }

  kj::Promise<void> setVelocity(const geometry_msgs::msg::Twist &velocity)
  {
    cmd_vel_pub_->publish(velocity);
    return kj::READY_NOW;
  }
  
  const AdaptedAction<create_action::Dock> dock;
  const AdaptedAction<create_action::DriveArc> drive_arc;
  const AdaptedAction<create_action::DriveDistance> drive_distance;
  const AdaptedAction<create_action::LedAnimation> led_animation;
  const AdaptedAction<create_action::NavigateToPosition> navigate_to;
  const AdaptedAction<create_action::RotateAngle> rotate;
  const AdaptedAction<create_action::Undock> undock;

  const create_msg::HazardDetectionVector::ConstSharedPtr &getHazardDetectionVector() const
  {
    usleep(2000);
    return latest_hazard_detection_;
  }

  const create_msg::IrIntensityVector::ConstSharedPtr &getCliffIntensityVector() const
  {
    usleep(2000);
    return latest_cliff_intensity_;
  }

  const create_msg::IrIntensityVector::ConstSharedPtr &getIrIntensityVector() const
  {
    usleep(2000);
    return latest_ir_intensity_;
  }

  const nav_msgs::msg::Odometry::ConstSharedPtr &getOdometry() const
  {
    usleep(2000);
    return latest_odom_;
  }

private:
  void onCliffIntensity(const create_msg::IrIntensityVector::ConstSharedPtr &msg)
  {
    latest_cliff_intensity_ = msg;
  }

  void onHazardDetection(const create_msg::HazardDetectionVector::ConstSharedPtr &msg)
  {
    latest_hazard_detection_ = msg;
  }

  void onIrIntensity(const create_msg::IrIntensityVector::ConstSharedPtr &msg)
  {
    latest_ir_intensity_ = msg;
  }
  
  void onOdom(const nav_msgs::msg::Odometry::ConstSharedPtr &msg)
  {
    latest_odom_ = msg;
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  
  rclcpp::Subscription<create_msg::HazardDetectionVector>::SharedPtr hazard_detection_sub_;
  rclcpp::Subscription<create_msg::IrIntensityVector>::SharedPtr cliff_intensity_sub_;
  rclcpp::Subscription<create_msg::IrIntensityVector>::SharedPtr ir_intensity_sub_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

  create_msg::HazardDetectionVector::ConstSharedPtr latest_hazard_detection_;
  create_msg::IrIntensityVector::ConstSharedPtr latest_cliff_intensity_;
  create_msg::IrIntensityVector::ConstSharedPtr latest_ir_intensity_;
  nav_msgs::msg::Odometry::ConstSharedPtr latest_odom_;
};

class Create3Server : public kipr::create3::Create3::Server
{
public:
  Create3Server(const std::shared_ptr<Create3Node> &node)
    : node_(node)
  {
  }

  kj::Promise<void> isConnected(IsConnectedContext context) override
  {
    auto response = context.getResults();
    response.setConnected(true);

    return kj::READY_NOW;
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

  kj::Promise<void> driveArc(DriveArcContext context) override
  {
    auto params = context.getParams();

    create_action::DriveArc::Goal goal;
    goal.translate_direction = params.getDirection();
    goal.angle = params.getAngle();
    goal.radius = params.getRadius();
    goal.max_translation_speed = params.getMaxLinearSpeed();

    return node_->drive_arc(goal).ignoreResult();
  }

  kj::Promise<void> driveDistance(DriveDistanceContext context) override
  {
    auto params = context.getParams();

    create_action::DriveDistance::Goal goal;
    goal.distance = params.getDistance();
    goal.max_translation_speed = params.getMaxLinearSpeed();

    return node_->drive_distance(goal).ignoreResult();
  }

  kj::Promise<void> ledAnimation(LedAnimationContext context) override
  {
    auto params = context.getParams();
    auto lightring = params.getLightring();

    create_action::LedAnimation::Goal goal;
    goal.animation_type = params.getAnimationType();

    std::array<irobot_create_msgs::msg::LedColor, 6> led_colors;
    led_colors[0].red = lightring.getLed0().getR();
    led_colors[0].green = lightring.getLed0().getG();
    led_colors[0].blue = lightring.getLed0().getB();

    led_colors[1].red = lightring.getLed1().getR();
    led_colors[1].green = lightring.getLed1().getG();
    led_colors[1].blue = lightring.getLed1().getB();

    led_colors[2].red = lightring.getLed2().getR();
    led_colors[2].green = lightring.getLed2().getG();
    led_colors[2].blue = lightring.getLed2().getB();

    led_colors[3].red = lightring.getLed3().getR();
    led_colors[3].green = lightring.getLed3().getG();
    led_colors[3].blue = lightring.getLed3().getB();

    led_colors[4].red = lightring.getLed4().getR();
    led_colors[4].green = lightring.getLed4().getG();
    led_colors[4].blue = lightring.getLed4().getB();

    led_colors[5].red = lightring.getLed5().getR();
    led_colors[5].green = lightring.getLed5().getG();
    led_colors[5].blue = lightring.getLed5().getB();

    goal.lightring.leds = led_colors;

    goal.max_runtime = rclcpp::Duration(params.getDuration().getSeconds(), params.getDuration().getNanoseconds());

    return node_->led_animation(goal).ignoreResult();
  }

  kj::Promise<void> navigateTo(NavigateToContext context) override
  {
    auto params = context.getParams();
    auto pose = params.getPose();

    create_action::NavigateToPosition::Goal goal;
    goal.goal_pose.pose.position.x = pose.getPosition().getX();
    goal.goal_pose.pose.position.y = pose.getPosition().getY();
    goal.goal_pose.pose.position.z = pose.getPosition().getZ();

    goal.goal_pose.pose.orientation.x = pose.getOrientation().getX();
    goal.goal_pose.pose.orientation.y = pose.getOrientation().getY();
    goal.goal_pose.pose.orientation.z = pose.getOrientation().getZ();
    goal.goal_pose.pose.orientation.w = pose.getOrientation().getW();

    goal.max_translation_speed = params.getMaxLinearSpeed();
    goal.max_rotation_speed = params.getMaxAngularSpeed();
    goal.achieve_goal_heading = params.getAchieveGoalHeading();

    return node_->navigate_to(goal).ignoreResult();
  }

  kj::Promise<void> rotate(RotateContext context) override
  {
    auto params = context.getParams();

    create_action::RotateAngle::Goal goal;
    goal.angle = params.getAngle();
    goal.max_rotation_speed = params.getMaxAngularSpeed();

    return node_->rotate(goal).ignoreResult();
  }

  kj::Promise<void> undock(UndockContext context) override
  {
    return node_->undock(create_action::Undock::Goal {}).ignoreResult();
  }

  kj::Promise<void> getCliffIntensityVector(GetCliffIntensityVectorContext context) override
  {
    auto response = context.getResults();

    const auto cliff_intensity = node_->getCliffIntensityVector();

    const auto readings = cliff_intensity->readings;

    auto cliff_intensity_vector = response.initCliffIntensityVector(readings.size());

    for (size_t i = 0; i < readings.size(); ++i)
    {
      const double timestamp = readings[i].header.stamp.sec + readings[i].header.stamp.nanosec / 1e9;
      cliff_intensity_vector[i].setFrameId(readings[i].header.frame_id);
      cliff_intensity_vector[i].setTimestamp(timestamp);
      cliff_intensity_vector[i].setIntensity(readings[i].value);
    }

    return kj::READY_NOW;
  }

  kj::Promise<void> getHazardDetectionVector(GetHazardDetectionVectorContext context) override
  {
    auto response = context.getResults();

    const auto hazard_detection = node_->getHazardDetectionVector();

    const auto detections = hazard_detection->detections;

    auto hazard_detection_vector = response.initHazardDetectionVector(detections.size());

    for (size_t i = 0; i < detections.size(); ++i)
    {
      const double timestamp = detections[i].header.stamp.sec + detections[i].header.stamp.nanosec / 1e9;
      hazard_detection_vector[i].setFrameId(detections[i].header.frame_id);
      hazard_detection_vector[i].setTimestamp(timestamp);
      hazard_detection_vector[i].setType(detections[i].type);
    }

    return kj::READY_NOW;
  }

  kj::Promise<void> getIrIntensityVector(GetIrIntensityVectorContext context) override
  {
    auto response = context.getResults();

    const auto ir_intensity = node_->getIrIntensityVector();

    const auto readings = ir_intensity->readings;

    auto ir_intensity_vector = response.initIrIntensityVector(readings.size());

    for (size_t i = 0; i < readings.size(); ++i)
    {
      const double timestamp = readings[i].header.stamp.sec + readings[i].header.stamp.nanosec / 1e9;
      ir_intensity_vector[i].setFrameId(readings[i].header.frame_id);
      ir_intensity_vector[i].setTimestamp(timestamp);
      ir_intensity_vector[i].setIntensity(readings[i].value);
    }

    return kj::READY_NOW;
  }

  kj::Promise<void> getOdometry(GetOdometryContext context) override
  {
    auto response = context.getResults();

    const auto odom = node_->getOdometry();

    auto pose = response.initPose();
    auto position = pose.initPosition();
    position.setX(odom->pose.pose.position.x);
    position.setY(odom->pose.pose.position.y);
    position.setZ(odom->pose.pose.position.z);

    auto orientation = pose.initOrientation();
    orientation.setX(odom->pose.pose.orientation.x);
    orientation.setY(odom->pose.pose.orientation.y);
    orientation.setZ(odom->pose.pose.orientation.z);
    orientation.setW(odom->pose.pose.orientation.w);

    auto velocity = response.initVelocity();
    velocity.setLinearX(odom->twist.twist.linear.x);
    velocity.setAngularZ(odom->twist.twist.angular.z);

    return kj::READY_NOW;
  }

private:
  std::shared_ptr<Create3Node> node_;
};


int main(int argc, char *const argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Create3Node>();
  auto server = capnp::EzRpcServer(kj::heap<Create3Server>(node), "*", 50051);

  while (rclcpp::ok())
  {
    server.getWaitScope().poll();
    rclcpp::spin_some(node);
  }

  rclcpp::shutdown();

  return EXIT_SUCCESS;
}