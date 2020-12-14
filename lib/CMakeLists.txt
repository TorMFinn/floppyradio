add_library(floppyradiolib
  drive_poller.cc
  player.cc
  module_loader.cc
  track_list.cc
  )

target_compile_options(floppyradiolib PUBLIC
  -std=c++20
  )

target_link_libraries(floppyradiolib PRIVATE
  libopenmpt
  ${Boost_LIBRARIES}
  ${BZIP2_LIBRARIES}
  ${portaudio_LIBRARY}
  SDL2::SDL2
  spdlog
  pthread
  )

target_include_directories(floppyradiolib PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../include
  )

target_include_directories(floppyradiolib PRIVATE
  ${libopenmpt_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIRS}
  )

#add_dependencies(floppyradiolib build_libopenmpt)
