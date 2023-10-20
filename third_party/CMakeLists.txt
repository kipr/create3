include(FetchContent)

FetchContent_Declare(capnproto
  URL https://github.com/capnproto/capnproto/archive/refs/tags/v1.0.0.tar.gz
  URL_HASH SHA256=bcd44dde78055313a7786cb6ab020cbef19b9045b53857f90cce101c9453f715
)

FetchContent_Declare(cxxopts
  URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.1.1.tar.gz
  URL_HASH SHA256=523175f792eb0ff04f9e653c90746c12655f10cb70f1d5e6d6d9491420298a08
)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(capnproto cxxopts googletest)

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

