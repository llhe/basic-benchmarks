cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
message("CMAKE_VERSION: ${CMAKE_VERSION}")
project(basic-benchmarks C CXX)

option(ENABLE_CCACHE      "whether to build with ccache"                ON)
option(ENABLE_ARCH_NATIVE "whether to build with -march=native"         ON)

message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

if(ENABLE_CCACHE)
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_C_COMPILER_LAUNCHER   ccache)
  endif(CCACHE_FOUND)
endif(ENABLE_CCACHE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

if(ENABLE_ARCH_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif(ENABLE_ARCH_NATIVE)


include(${PROJECT_SOURCE_DIR}/third_party/third_party.cmake)
include_directories("${PROJECT_SOURCE_DIR}/benchmark")
add_subdirectory(benchmark)
