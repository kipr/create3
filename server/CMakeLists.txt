find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(create3_server
  src/kipr/create3/server/main.cpp
)

ament_target_dependencies(create3_server
  rclcpp
)