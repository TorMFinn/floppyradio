# Generate path to test modules

set(MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/assets/test-module.xm")
set(MODULE_PATH_BZIP2 "${CMAKE_CURRENT_LIST_DIR}/assets/test-module.xm.bz2")
configure_file(${CMAKE_CURRENT_LIST_DIR}/paths.hpp.in ${CMAKE_CURRENT_LIST_DIR}/paths.hpp @ONLY)

add_executable(test-module
  test-module.cc
  )

target_link_libraries(test-module
  floppyradiolib
  GTest::gtest
  ${libopenmpt_LIBRARY}
  )

target_include_directories(test-module PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  )

add_executable(test-track-list
  test-track-list.cc
  )

target_link_libraries(test-track-list
  GTest::gtest
  floppyradiolib
  )

add_test(NAME TestModule COMMAND test-module)
add_test(NAME TestTrackList COMMAND test-track-list)
