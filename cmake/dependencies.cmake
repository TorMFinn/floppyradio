include(${CMAKE_CURRENT_LIST_DIR}/libopenmpt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_udisks2.cmake)

find_package(Boost REQUIRED COMPONENTS
  filesystem
  )

#find_library(pulseaudio_LIBRARIES NAMES pulse pulse-simple REQUIRED)
find_library(portaudio_LIBRARY NAMES portaudio REQUIRED)
find_package(SDL2 REQUIRED)

find_package(BZip2 REQUIRED)

add_subdirectory(ext/spdlog)

if (FLOPPYRADIO_BUILD_TESTS)
  message(STATUS "Enabled tests, looking for GTest")
  find_package(GTest CONFIG REQUIRED)
endif()
