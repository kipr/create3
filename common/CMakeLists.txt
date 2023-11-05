capnp_generate_cpp(SCHEMA_SOURCES SCHEMA_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/schema/kipr/create3/create3.capnp)

add_library(create3_common STATIC ${SCHEMA_SOURCES} ${SCHEMA_HEADERS})
target_link_libraries(create3_common PUBLIC CapnProto::capnp)
target_include_directories(create3_common PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/schema)

set_target_properties(
  create3_common
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

install(TARGETS create3_common
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES ${SCHEMA_HEADERS} DESTINATION include/kipr/create3)
