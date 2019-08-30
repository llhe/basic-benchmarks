add_executable(benchmark_bin
  concurrency/add_benchmark.cc
  concurrency/sharing_benchmark.cc
  memory/memory_benchmark.cc
  stl/container_benchmark.cc
  stl/algorithm_benchmark.cc
  string/string_benchmark.cc
  queue/queue_benchmark.cc
)

target_link_libraries(benchmark_bin
  absl::base absl::synchronization absl::strings
  benchmark_main benchmark pthread)
