#include <stdlib.h>
#include <rclcpp/rclcpp.hpp>

class Create3 : public rclcpp::Node
{
public:
  MinimalPublisher()
    : Node("minimal_publisher")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }
};


int main(int argc, char *const argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MinimalPublisher>();

  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
  }

  rclcpp::shutdown();

  return EXIT_SUCCESS;
}