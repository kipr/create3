capnp_generate_cpp(SCHEMA_SOURCES SCHEMA_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/schema/kipr/create3/create3.capnp)

add_library(${CMAKE_PROJECT_NAME}_common STATIC ${SCHEMA_SOURCES} ${SCHEMA_HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME}_common PUBLIC CapnProto::capnp)
target_include_directories(${CMAKE_PROJECT_NAME}_common PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/schema)

install(TARGETS ${CMAKE_PROJECT_NAME}_common
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES ${SCHEMA_HEADERS} DESTINATION include/kipr/${CMAKE_PROJECT_NAME})
