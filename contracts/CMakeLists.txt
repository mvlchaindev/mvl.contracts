cmake_minimum_required( VERSION 3.5 )

project(contracts)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_subdirectory(libraries)
#link_libraries(eoslib)

add_subdirectory(mvl.bios)
add_subdirectory(mvl.token)
add_subdirectory(mvl.system)
