add_executable(floppyradio
  main.cpp
  )

target_link_libraries(floppyradio PRIVATE
  floppyradiolib
  pthread
  )
