include(FetchContent)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(irobot_create_msgs REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}_server
  src/kipr/create3/server/main.cpp
)

ament_target_dependencies(${CMAKE_PROJECT_NAME}_server
  rclcpp
  std_msgs
  geometry_msgs
  irobot_create_msgs
)

message(STATUS "irobot_create_msgs_INCLUDE_DIRS: ${irobot_create_msgs}")

install(TARGETS
  ${CMAKE_PROJECT_NAME}_server
  DESTINATION lib/${PROJECT_NAME}
)