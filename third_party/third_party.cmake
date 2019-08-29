set(NNSERV_THIRD_PARTY_DIR "${PROJECT_BINARY_DIR}/third_party" CACHE STRING "Third party libraries download & build directories.")

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
  set(LIBSUFFIX 64)
else()
  set(LIBSUFFIX "")
endif()

include(${PROJECT_SOURCE_DIR}/third_party/gflags/gflags.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/benchmark/benchmark.cmake)
