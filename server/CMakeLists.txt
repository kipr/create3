find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}_server
  src/kipr/create3/server/main.cpp
)

ament_target_dependencies(${CMAKE_PROJECT_NAME}_server
  rclcpp
  std_msgs
)

install(TARGETS
  ${CMAKE_PROJECT_NAME}_server
  DESTINATION lib/${PROJECT_NAME}
)