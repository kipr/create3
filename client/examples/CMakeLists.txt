function(add_example name)
  add_executable(${name} ${ARGN})
  target_link_libraries(${name} ${CMAKE_PROJECT_NAME}_client)
endfunction()

add_example(01_connect 01_connect.c)
add_example(02_dock_undock 02_dock_undock.c)
add_example(03_move_velocity 03_move_velocity.c)