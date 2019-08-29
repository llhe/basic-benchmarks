add_executable(benchmark_bin
  atomic/atomic_benchmark.cc
)
target_link_libraries(benchmark_bin benchmark_main benchmark pthread)
