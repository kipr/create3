cmake_policy(SET CMP0048 NEW)

project(create3 VERSION 0.1.0)

cmake_minimum_required(VERSION 3.5)

option("client" "Build the client" ON)
option("server" "Build the server" ON)
option("cmder" "Build the cmder" ON)
option("tests" "Enable tests" ON)
option("debug" "Enable debugging" OFF)

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
    add_executable(${CMAKE_PROJECT_NAME}_${name} ${file})
    target_link_libraries(${CMAKE_PROJECT_NAME}_${name} GTest::gtest_main ${ARGN})
    gtest_discover_tests(${CMAKE_PROJECT_NAME}_${name})
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

