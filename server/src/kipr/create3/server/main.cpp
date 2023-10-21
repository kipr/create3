#include <stdlib.h>
#include <chrono>
#include <rclcpp/rclcpp.hpp>

#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

class Create3 : public rclcpp::Node
{
public:
  Create3()
    : Node("minimal_publisher")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&Create3::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
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