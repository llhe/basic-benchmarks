include(ExternalProject)

set(ABSEIL_SRCS_DIR    "${BASIC_BENCHMARK_THIRD_PARTY_DIR}/abseil")

# Must keep consistent version with TensorFlow
# https://github.com/abseil/abseil-cpp/archive/43ef2148c0936ebf7cb4be6b19927a9d9d145b8f.tar.gz
set(ABSEIL_URL     "https://cnbj1-inner-fds.api.xiaomi.net/third-party/abseil/43ef2148c0936ebf7cb4be6b19927a9d9d145b8f.tar.gz")
set(ABSEIL_HASH    "SHA256=acd93f6baaedc4414ebd08b33bebca7c7a46888916101d8c0b8083573526d070")

include(${BASIC_BENCHMARK_THIRD_PARTY_SRC_DIR}/download-project/DownloadProject.cmake)
download_project(PROJ abseil
  URL_HASH          "${ABSEIL_HASH}"
  URL               "${ABSEIL_URL}"
  PREFIX            "${ABSEIL_SRCS_DIR}"
  QUIET
)

set(GTEST_BUILD_TESTING_BACKUP ${BUILD_TESTING})
set(BUILD_TESTING OFF)

add_subdirectory(${abseil_SOURCE_DIR} ${abseil_BINARY_DIR})
include_directories("${abseil_SOURCE_DIR}")

set(BUILD_TESTING ${GTEST_BUILD_TESTING_BACKUP})
