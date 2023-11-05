include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(capnproto
  URL https://github.com/capnproto/capnproto/archive/refs/tags/v1.0.0.tar.gz
  URL_HASH SHA256=bcd44dde78055313a7786cb6ab020cbef19b9045b53857f90cce101c9453f715
)

FetchContent_Declare(clipp
  URL https://github.com/muellan/clipp/archive/refs/tags/v1.2.3.tar.gz
  URL_HASH SHA256=73da8e3d354fececdea99f7deb79d0343647349563ace3eafb7f4cca6e86e90b
)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  URL_HASH SHA256=edd885a1ab32b6999515a880f669efadb80b3f880215f315985fa3f6eca7c4d3
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(capnproto clipp googletest)

# Set -fPIC
set_target_properties(
  capnp-rpc
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
  capnp
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
  kj-async
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
  kj
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

add_executable(CapnProto::capnp_tool IMPORTED)
set_target_properties(CapnProto::capnp_tool PROPERTIES
  IMPORTED_LOCATION "${capnproto_BINARY_DIR}/c++/src/capnp/capnp${CMAKE_EXECUTABLE_SUFFIX}"
)

add_executable(CapnProto::capnpc_cpp IMPORTED)
set_target_properties(CapnProto::capnpc_cpp PROPERTIES
  IMPORTED_LOCATION "${capnproto_BINARY_DIR}/c++/src/capnp/capnpc-c++${CMAKE_EXECUTABLE_SUFFIX}"
)

add_executable(CapnProto::capnpc_capnp IMPORTED)
set_target_properties(CapnProto::capnpc_capnp PROPERTIES
  IMPORTED_LOCATION "${capnproto_BINARY_DIR}/c++/src/capnp/capnpc-capnp${CMAKE_EXECUTABLE_SUFFIX}"
)


add_library(clipp INTERFACE)

target_include_directories(clipp INTERFACE
  $<BUILD_INTERFACE:${clipp_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)