add_executable(${CMAKE_PROJECT_NAME}_cmder src/kipr/create3/cmder/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_cmder PRIVATE ${CMAKE_PROJECT_NAME}_client clipp)

install(TARGETS ${CMAKE_PROJECT_NAME}_cmder
  RUNTIME DESTINATION bin
)