cmake_minimum_required(VERSION 3.14)
project(floppyplayer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/build_options.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(lib)
add_subdirectory(bin)

if (FLOPPYRADIO_BUILD_TESTS)
  enable_testing()
  add_subdirectory(lib/tests)
endif()
