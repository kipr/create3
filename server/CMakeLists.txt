include(FetchContent)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(irobot_create_msgs REQUIRED)

add_executable(create3_server
  src/kipr/create3/server/main.cpp
)

target_link_libraries(create3_server
  create3_common
  CapnProto::capnp-rpc
)

ament_target_dependencies(create3_server
  rclcpp
  rclcpp_action
  std_msgs
  nav_msgs
  geometry_msgs
  irobot_create_msgs
)

message(STATUS "irobot_create_msgs_INCLUDE_DIRS: ${irobot_create_msgs}")

install(TARGETS
  create3_server
  DESTINATION lib/${PROJECT_NAME}
)