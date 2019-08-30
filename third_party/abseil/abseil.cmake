INCLUDE(ExternalProject)

set(ABSEIL_SRCS_DIR    "${BASIC_BENCHMARK_THIRD_PARTY_DIR}/abseil")

set(ABSEIL_URL     "https://github.com/abseil/abseil-cpp/archive/20190808.zip")
set(ABSEIL_HASH    "SHA256=0b62fc2d00c2b2bc3761a892a17ac3b8af3578bd28535d90b4c914b0a7460d4e")

ExternalProject_Add(
  abseil-cpp
  URL_HASH          "${ABSEIL_HASH}"
  URL               "${ABSEIL_URL}"
  PREFIX            "${ABSEIL_SRCS_DIR}"
  BINARY_DIR        ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

add_subdirectory(${ABSEIL_SRCS_DIR}/src/abseil-cpp)
