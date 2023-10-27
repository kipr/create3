add_library(${CMAKE_PROJECT_NAME}_client SHARED
  src/kipr/create3/client/client_c.cpp
  src/kipr/create3/client/Client.cpp
  src/kipr/create3/client/ClientImpl.cpp
  src/kipr/create3/client/duration_c.cpp
  src/kipr/create3/client/euler_c.cpp
  src/kipr/create3/client/led_animation_type_c.cpp
  src/kipr/create3/client/test_utils.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}_client PUBLIC include)
target_link_libraries(${CMAKE_PROJECT_NAME}_client PRIVATE ${CMAKE_PROJECT_NAME}_common CapnProto::capnp-rpc)

add_gtest(test/kipr/create3/client/Client.test.cpp ${CMAKE_PROJECT_NAME}_client ${CMAKE_PROJECT_NAME}_common CapnProto::capnp-rpc)

install(TARGETS ${CMAKE_PROJECT_NAME}_client
  LIBRARY DESTINATION lib
)

install(DIRECTORY include/kipr DESTINATION include)

add_subdirectory(examples)