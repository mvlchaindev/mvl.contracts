add_contract(mvl.system mvl.system ${CMAKE_CURRENT_SOURCE_DIR}/src/mvl.system.cpp)

target_include_directories(mvl.system
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../mvl.token/include)

set_target_properties(mvl.system
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
