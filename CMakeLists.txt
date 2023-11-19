cmake_policy(SET CMP0048 NEW)

set(CREATE3_VERSION_MAJOR 0)
set(CREATE3_VERSION_MINOR 1)
set(CREATE3_VERSION_PATCH 0)

project(create3 VERSION ${CREATE3_VERSION_MAJOR}.${CREATE3_VERSION_MINOR}.${CREATE3_VERSION_PATCH})

cmake_minimum_required(VERSION 3.5)

option("client" "Build the client" ON)
option("server" "Build the server" ON)
option("cmder" "Build the cmder" ON)
option("tests" "Enable tests" ON)
option("debug" "Enable debugging" OFF)
option(package_debian "Build a debian package" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (debug)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

if (tests)
  enable_testing()
  include(GoogleTest)
  function(add_gtest file)
    message(STATUS "Adding test ${file}")
    # convert the file path into a target name by replacing / with _
    string(REPLACE "/" "_" name ${file})
    add_executable(create3_${name} ${file})
    target_link_libraries(create3_${name} GTest::gtest_main ${ARGN})
    gtest_discover_tests(create3_${name})
  endfunction()
elseif()
  function(add_gtest name)
  endfunction()
endif()

add_subdirectory(third_party)
add_subdirectory(common)

if(client)
    add_subdirectory(client)
    if (cmder)
      add_subdirectory(cmder)
    endif()
endif()



if(server)
    add_subdirectory(server)
endif()

set(CPACK_PACKAGE_NAME create3)
set(CPACK_GENERATOR DEB ZIP)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KISS Institute for Practical Robotics")
set(CPACK_PACKAGE_VERSION_MAJOR ${CREATE3_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CREATE3_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CREATE3_VERSION_PATCH})
if(package_debian)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)
elseif()
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
endif()
include(CPack)

