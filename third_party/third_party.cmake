set(NNSERV_THIRD_PARTY_DIR "${PROJECT_BINARY_DIR}/third_party" CACHE STRING "Third party libraries download & build directories.")

include(${PROJECT_SOURCE_DIR}/third_party/gflags/gflags.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/benchmark/benchmark.cmake)
