function(add_example name)
  add_executable(${name} ${ARGN})
  target_link_libraries(${name} create3_client)
endfunction()

add_example(01_connect 01_connect.c)
add_example(02_dock_undock 02_dock_undock.c)
add_example(03_move_velocity 03_move_velocity.c)
add_example(04_rotate 04_rotate.c)
add_example(05_arc 05_arc.c)
add_example(06_led_animation_test 06_led_animation_test.c)
add_example(07_sensors 07_sensors.c)