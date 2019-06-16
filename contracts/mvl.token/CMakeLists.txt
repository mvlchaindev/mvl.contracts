add_contract(mvl.token mvl.token ${CMAKE_CURRENT_SOURCE_DIR}/src/mvl.token.cpp)

target_include_directories(mvl.token
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(mvl.token
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
