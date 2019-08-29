add_executable(benchmark_bin
  concurrency/add_benchmark.cc
  memory/memory_benchmark.cc
  stl/container_benchmark.cc
  stl/algorithm_benchmark.cc
  string/string_benchmark.cc
)
target_link_libraries(benchmark_bin benchmark_main benchmark pthread)
