#include <cstring>
#include <cstdlib>

#include <memory>
#include <vector>
#include <iostream>

#include <benchmark/benchmark.h>

struct SharedBuffer {
  int paddings;
  int32_t *buffer;

  SharedBuffer(int shards, int paddings) : paddings(paddings) {
    int total_size = shards * sizeof(int32_t) * (1 + paddings);
    if (posix_memalign((void**) &buffer, 64, total_size)) {
      std::cerr << "Couldn't allocate memory" << std::endl;
    }
  }

  ~SharedBuffer() {
    free(buffer);
  }

  inline int32_t *addr(int shard) {
    return buffer + shard * (paddings + 1);
  }
};

class BM_Sharing: public benchmark::Fixture {
  public:
    std::unique_ptr<SharedBuffer> buffer;

    void SetUp(const ::benchmark::State& state) {
      int shards = state.threads;
      int paddings = state.range(0);
      if (state.thread_index == 0) {
        buffer.reset(new SharedBuffer(shards, paddings));
      }
    }
};

BENCHMARK_DEFINE_F(BM_Sharing, FalseSharing)(benchmark::State& state) {
  const int n = 1000000;
  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      buffer->addr(state.thread_index)[0]++;
    }
  }
  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * n,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(8)
  ->DenseRange(0, 20, 2);
BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(16)
  ->DenseRange(0, 20, 2);
