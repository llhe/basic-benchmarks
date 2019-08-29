#include <queue>
#include <vector>
#include <random>
#include <mutex>

#include <benchmark/benchmark.h>

static void BM_StdQueueSeqPush(benchmark::State& state) {
  std::queue<int> q;
  int n = state.range(0);

  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      q.push(i);
    }

    state.PauseTiming();
    for (int i = 0; i < n; ++i) {
      q.pop();
    }
    state.ResumeTiming();
  }
  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * n,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

BENCHMARK(BM_StdQueueSeqPush)->RangeMultiplier(10)->Range(10, 10000);

static void BM_StdQueueSeqPushLocked(benchmark::State& state) {
  std::queue<int> q;
  int n = state.range(0);
  std::mutex lck;

  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      std::lock_guard<std::mutex> guard(lck);
      q.push(i);
    }

    state.PauseTiming();
    for (int i = 0; i < n; ++i) {
      q.pop();
    }
    state.ResumeTiming();
  }
  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * n,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

BENCHMARK(BM_StdQueueSeqPushLocked)->RangeMultiplier(10)->Range(10, 10000);

static void BM_StdQueueSeqPop(benchmark::State& state) {
  std::queue<int> q;
  int n = state.range(0);

  for (auto _ : state) {
    state.PauseTiming();
    for (int i = 0; i < n; ++i) {
      q.push(i);
    }
    state.ResumeTiming();

    for (int i = 0; i < n; ++i) {
      q.pop();
    }
  }

  state.counters["Op/s"] = benchmark::Counter(
      static_cast<int64_t>(state.iterations()) * n,
      benchmark::Counter::kIsRate,
      benchmark::Counter::OneK::kIs1000);
}

BENCHMARK(BM_StdQueueSeqPop)->RangeMultiplier(10)->Range(10, 10000);
