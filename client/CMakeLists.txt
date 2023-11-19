add_library(create3_client STATIC
  src/kipr/create3/client/bump_sensor_c.cpp
  src/kipr/create3/client/client_c.cpp
  src/kipr/create3/client/Client.cpp
  src/kipr/create3/client/ClientImpl.cpp
  src/kipr/create3/client/duration_c.cpp
  src/kipr/create3/client/euler_c.cpp
  src/kipr/create3/client/led_animation_type_c.cpp
  src/kipr/create3/client/test_utils.cpp
)

target_include_directories(create3_client PUBLIC include)
target_link_libraries(create3_client PRIVATE create3_common CapnProto::capnp-rpc)

add_gtest(test/kipr/create3/client/Client.test.cpp create3_client create3_common CapnProto::capnp-rpc)

install(TARGETS create3_client
  LIBRARY DESTINATION lib
)

install(DIRECTORY include/kipr DESTINATION include)

add_subdirectory(examples)