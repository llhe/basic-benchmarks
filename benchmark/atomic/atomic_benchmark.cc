#include <benchmark/benchmark.h>
#include <atomic>
#include <mutex>

#include "utils.h"

template <typename T>
static void BM_RawNumberAdd(benchmark::State& state) {
  T counter = 0;

  for (auto _ : state) {
    // Adding loop will add ~1 ns overhead
    // for (int i = N; i > 0; --i)
    ++counter;
  }
  DoNotOptimizeAway(counter);
}

static void BM_RawInt32Add(benchmark::State& state) {
  BM_RawNumberAdd<int32_t>(state);
}

static void BM_RawInt64Add(benchmark::State& state) {
  BM_RawNumberAdd<int64_t>(state);
}

static void BM_RawFloat32Add(benchmark::State& state) {
  BM_RawNumberAdd<float>(state);
}

// Loop add overhead of branch misprediction
template <typename T>
static void BM_RawNumberAddLooped(benchmark::State& state) {
  int N = state.range(0);
  T counter = 0;

  for (auto _ : state) {
    for (int i = N; i > 0; --i) {
      ++counter;
    }
  }
  DoNotOptimizeAway(counter);
}

static void BM_RawInt32AddLooped(benchmark::State& state) {
  BM_RawNumberAddLooped<int32_t>(state);
}

static void BM_RawInt64AddLooped(benchmark::State& state) {
  BM_RawNumberAddLooped<int64_t>(state);
}

template <typename T>
static void BM_AtomicNumberAddST(benchmark::State& state) {
  std::atomic<T> counter(0);

  for (auto _ : state) {
    ++counter;
  }

  DoNotOptimizeAway(counter);
}

static void BM_AtomicInt32AddST(benchmark::State& state) {
  BM_AtomicNumberAddST<int32_t>(state);
}

static void BM_AtomicInt64AddST(benchmark::State& state) {
  BM_AtomicNumberAddST<int64_t>(state);
}

template <typename T>
static void BM_MutexGuardedNumberAddST(benchmark::State& state) {
  std::mutex lck;
  std::atomic<T> counter(0);

  for (auto _ : state) {
    std::lock_guard<std::mutex> guard(lck);
    ++counter;
  }

  DoNotOptimizeAway(counter);
}

static void BM_MutexGuardedInt32AddST(benchmark::State& state) {
  BM_MutexGuardedNumberAddST<int32_t>(state);
}

BENCHMARK(BM_RawInt32Add);
BENCHMARK(BM_RawInt64Add);
BENCHMARK(BM_RawFloat32Add);
BENCHMARK(BM_RawInt32AddLooped)->Arg(1);
BENCHMARK(BM_RawInt64AddLooped)->Arg(1);
BENCHMARK(BM_AtomicInt32AddST);
BENCHMARK(BM_AtomicInt64AddST);
BENCHMARK(BM_MutexGuardedInt32AddST);
