include(${CMAKE_CURRENT_LIST_DIR}/libopenmpt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_udisks2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_spdlog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_libsidplayfp.cmake)

find_package(Boost REQUIRED COMPONENTS
  filesystem
  )

find_library(portaudio_LIBRARY NAMES portaudio REQUIRED)
find_package(SDL2 REQUIRED)
find_package(BZip2 REQUIRED)

if (FLOPPYRADIO_BUILD_TESTS)
  message(STATUS "Enabled tests")
  include(${CMAKE_CURRENT_LIST_DIR}/find_gtest.cmake)
endif()
