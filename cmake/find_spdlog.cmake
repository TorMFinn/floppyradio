include(FetchContent)

FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.8.5.tar.gz
  )

message(STATUS "Fetching spdlog")
FetchContent_MakeAvailable(spdlog)
