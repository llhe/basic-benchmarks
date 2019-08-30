#include <cstring>
#include <cstdlib>

#include <memory>
#include <thread>
#include <vector>
#include <iostream>

#include <benchmark/benchmark.h>
#include <absl/synchronization/barrier.h>
#include <absl/synchronization/blocking_counter.h>

struct SharedBuffer {
  const int paddings;
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

  inline void UpdateShard(int shard) {
    ++buffer[shard * (paddings + 1)];
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
  constexpr int64_t kIterNum = 1000000;
  for (auto _ : state) {
    // FIXME this line does not incur intensive contention
    // due to the sync?
    for (int64_t i = 0; i < kIterNum; ++i) {
      buffer->UpdateShard(state.thread_index);
    }
  }
  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * kIterNum,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(8)
  ->DenseRange(0, 20, 2);
BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(16)
  ->DenseRange(0, 20, 2);


static void BM_FalseSharing(benchmark::State& state) {
  constexpr int64_t kIterNum = 1000000;
  int thread_num = state.range(0);
  int paddings = state.range(1);
  SharedBuffer buffer(thread_num, paddings);

  for (auto _ : state) {
    state.PauseTiming();
    std::vector<std::thread> workers;
    workers.reserve(thread_num);

    absl::Barrier barrier(thread_num + 1);
    absl::BlockingCounter counter(thread_num + 1);

    for (int i = 0; i < thread_num; ++i) {
      workers.emplace_back(std::thread([&](int tid) {
            barrier.Block();
            for (int64_t i = 0; i < kIterNum; ++i) {
              buffer.UpdateShard(tid);
            }
            counter.DecrementCount();
            }, i));
    }

    barrier.Block();  // Wait all threads are ready
    state.ResumeTiming();

    counter.DecrementCount();  // Start to work
    counter.Wait();

    state.PauseTiming();
    for (int i = 0; i < thread_num; ++i) {
      workers[i].join();
    }
    state.ResumeTiming();
  }

  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * kIterNum,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

// BENCHMARK(BM_FalseSharing)->Args({8, 0})
// ->Args({8, 2})
// ->Args({8, 4})
// ->Args({8, 8})
// ->Args({8, 16})
// ->Args({8, 32});
