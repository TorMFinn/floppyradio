add_library(floppyradiolib
  DriveMounter.cpp
  SDLAudioDevice.cpp
  Player.cpp
  ModuleLoader.cpp
  TrackList.cpp
  Display.cpp
  SoftwareDisplay.cpp
  StarField.cpp
  )

target_compile_options(floppyradiolib PUBLIC
  -std=c++20
  )

target_link_libraries(floppyradiolib PRIVATE
  libopenmpt
  libsidplayfp
  ${Boost_LIBRARIES}
  ${BZIP2_LIBRARIES}
  ${portaudio_LIBRARY}
  ${UDISKS2_LIBRARIES}
  ${GLFW_LIBRARIES}
  SDL2::SDL2
  spdlog::spdlog
  pthread
  )

target_include_directories(floppyradiolib PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../include
  )

target_include_directories(floppyradiolib PRIVATE
  ${libopenmpt_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIRS}
  ${UDISKS2_INCLUDE_DIRS}
  )

add_executable(floppyradio
  main.cpp
  )

target_link_libraries(floppyradio
  floppyradiolib
  pthread
  )
