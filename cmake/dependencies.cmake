include(FetchContent)
include(ExternalProject)

FetchContent_Declare(
  libopenmpt
  URL https://lib.openmpt.org/files/libopenmpt/src/libopenmpt-0.5.2+release.makefile.tar.gz
  )

FetchContent_GetProperties(libopenmpt)
if(NOT libopenmpt_POPULATED)
  message(STATUS "Downloading libopenmpt")
  FetchContent_Populate(libopenmpt)
  message(STATUS "Populated libopenmpt")
  set(libopenmpt_LIBRARY ${libopenmpt_BINARY_DIR}/lib/libopenmpt.a)
  set(libopenmpt_INCLUDE_DIR ${libopenmpt_BINARY_DIR}/include)

  message(STATUS "DIR: ${libopenmpt_BINARY_DIR}")
  add_custom_target(build_libopenmpt
    COMMAND make CONFIG=clang OPENMPT123=0 EXAMPLES=0 SHARED_LIB=0 TEST=0 PREFIX=${libopenmpt_BINARY_DIR}/ install
    COMMENT "Building libopenmpt"
    WORKING_DIRECTORY ${libopenmpt_SOURCE_DIR}
    BYPRODUCTS ${libopenmpt_LIBRARY}
    VERBATIM
    )
  
endif()
