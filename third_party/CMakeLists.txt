include(ExternalProject)

ExternalProject_Add(capnproto
  URL https://capnproto.org/capnproto-c++-1.0.1.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/capnproto
  CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/capnproto
  BUILD_COMMAND make -j6
  INSTALL_COMMAND make install
  BUILD_IN_SOURCE 1
)

find_package(CapnProto REQUIRED
  PATHS "${CMAKE_CURRENT_BINARY_DIR}/capnproto/lib/cmake/CapnProto"
)