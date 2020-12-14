include(ExternalProject)

# Dependecies required for libopenmpt
find_library(libogg_LIBRARY NAMES ogg REQUIRED)
find_library(libvorbis_LIBRARY NAMES vorbis REQUIRED)
find_library(libvorbisfile_LIBRARY NAMES vorbisfile REQUIRED)
find_library(zlib_LIBRARY NAMES z REQUIRED)
set(libopenmpt_INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

ExternalProject_add(libopenmpt_ext
  URL https://lib.openmpt.org/files/libopenmpt/src/libopenmpt-0.5.2+release.makefile.tar.gz
  DOWNLOAD_NO_PROGRESS true
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE true
  BUILD_COMMAND make CONFIG=clang OPENMPT123=0 EXAMPLES=0 SHARED_LIB=0 TEST=0 NO_MPG123=1 PREFIX=${libopenmpt_INSTALL_DIR} install
  INSTALL_COMMAND ""
  )

add_library(libopenmpt STATIC IMPORTED
  ${libopenmpt_INSTALL_DIR}/lib/libopenmpt.a
  )

target_include_directories(libopenmpt INTERFACE
  ${libopenmpt_INSTALL_DIR}/include
  )

set_target_properties(libopenmpt PROPERTIES
  IMPORTED_LOCATION ${libopenmpt_INSTALL_DIR}/lib/libopenmpt.a
  )

target_link_libraries(libopenmpt INTERFACE
  ${libogg_LIBRARY}
  ${libvorbis_LIBRARY}
  ${libvorbisfile_LIBRARY}
  ${zlib_LIBRARY}
  )
