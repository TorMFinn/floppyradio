include(FetchContent)

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG 23ef295
  )

message(STATUS "Fetching googletest")
FetchContent_MakeAvailable(googletest)
