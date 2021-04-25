include(ExternalProject)

set(libsidplayfp_INSTALL_DIR ${CMAKE_BINARY_DIR}/install)
ExternalProject_add(libsidplayfp_ext
  URL https://github.com/libsidplayfp/libsidplayfp/releases/download/v2.1.2/libsidplayfp-2.1.2.tar.gz
  DOWNLOAD_NO_PROGRESS true
  CONFIGURE_COMMAND ./configure --prefix=${libsidplayfp_INSTALL_DIR}
  BUILD_IN_SOURCE true
  BUILD_COMMAND make install
  INSTALL_COMMAND ""
  )

add_library(libsidplayfp STATIC IMPORTED
  ${libsidplayfp_INSTALL_DIR}/lib/libsidplayfp.a
  )

target_include_directories(libsidplayfp INTERFACE
  ${libsidplayfp_INSTALL_DIR}/include
  )

set_target_properties(libsidplayfp PROPERTIES
  IMPORTED_LOCATION ${libsidplayfp_INSTALL_DIR}/lib/libsidplayfp.a
  )
