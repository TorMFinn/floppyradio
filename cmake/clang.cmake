# This chooses clang as the compiler and enables C++20 modules support

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

set(CMAKE_CXX_FLAGS "-std=c++20 -fimplicit-modules -fmodules-ts -fimplicit-module-maps")
