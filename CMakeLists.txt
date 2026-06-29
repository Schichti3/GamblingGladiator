cmake_minimum_required(VERSION 3.20)

project(
  GamblingGladiator
  VERSION 0.1.0
  LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED IMPORTED_TARGET raylib)
pkg_check_modules(ENET REQUIRED IMPORTED_TARGET libenet)

add_subdirectory(src/os_utils)
add_subdirectory(src/network/thread)
add_subdirectory(src/network/client)
add_subdirectory(src/network/server)
add_subdirectory(src/dedicated_server)
add_subdirectory(src/game/dll)
add_subdirectory(src/game/exe)

