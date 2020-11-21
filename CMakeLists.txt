cmake_minimum_required(VERSION 3.18)
project(floppyplayer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/dependencies.cmake)

find_package(Boost REQUIRED COMPONENTS
  filesystem
  )

add_executable(floppyradio
  src/main.cpp
  )

add_library(floppyradiolib
  src/drive_poller.cc
  src/player.cc
  src/track_list.cpp
  )

target_link_libraries(floppyradiolib PRIVATE
  ${libopenmpt_LIBRARY}
  ${Boost_LIBRARIES}
  pthread
  )

target_link_libraries(floppyradio PRIVATE
  floppyradiolib
  pthread
  )

target_include_directories(floppyradiolib PUBLIC
  ${libopenmpt_INCLUDE_DIR}
  )

add_dependencies(floppyradiolib build_libopenmpt)
