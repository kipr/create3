add_executable(01_connect 01_connect.c)
target_link_libraries(01_connect ${CMAKE_PROJECT_NAME}_client)