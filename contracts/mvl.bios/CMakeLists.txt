add_contract(mvl.bios mvl.bios ${CMAKE_CURRENT_SOURCE_DIR}/src/mvl.bios.cpp)

target_include_directories(mvl.bios
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(mvl.bios
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
