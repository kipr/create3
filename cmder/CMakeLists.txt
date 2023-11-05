add_executable(create3_cmder src/kipr/create3/cmder/main.cpp)
target_link_libraries(create3_cmder PRIVATE create3_client clipp)

install(TARGETS create3_cmder
  RUNTIME DESTINATION bin
)