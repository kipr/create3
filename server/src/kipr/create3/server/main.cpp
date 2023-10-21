#include <stdlib.h>
#include <chrono>
#include <rclcpp/rclcpp.hpp>

#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <irobot_create_msgs/msg/wheel_vels.hpp>

using namespace std::chrono_literals;

class Create3 : public rclcpp::Node
{
public:
  Create3()
    : Node("create3")
  {
    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    timer_ = this->create_wall_timer(
      1000ms, std::bind(&Create3::timer_callback, this));
  }

private:
  bool stop_go = false;

  void timer_callback()
  {
    stop_go = !stop_go;
    
    geometry_msgs::msg::Twist msg;
    if (stop_go)
    {
      msg.linear.x = 0.1;
      msg.angular.z = 0.1;
    }
    else
    {
      msg.linear.x = 0.0;
      msg.angular.z = 0.0;
    }

    RCLCPP_INFO(this->get_logger(), "Publishing: '%f'", msg.linear.x);
    cmd_vel_pub_->publish(msg);
  }
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  size_t count_ = 1;
};


int main(int argc, char *const argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<Create3>();

  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
  }

  rclcpp::shutdown();

  return EXIT_SUCCESS;
}