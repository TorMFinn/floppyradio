# reSID is a guddie SID emulator to be used for playing SID files
include(ExternalProject)

ExternalProject_add(resid_ext
  URL http://www.zimmers.net/anonftp/pub/cbm/crossplatform/emulators/resid/resid-0.16.tar.gz
  DOWNLOAD_NO_PROGRESS true
  CONFIGURE_COMMAND
