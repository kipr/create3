project(create3)

cmake_minimum_required(VERSION 3.5)

option("client" "Build the client" ON)
option("server" "Build the server" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(third_party)
add_subdirectory(common)

if(client)
    add_subdirectory(client)
endif()

if(server)
    add_subdirectory(server)
endif()