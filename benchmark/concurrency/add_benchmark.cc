#include <benchmark/benchmark.h>
#include <atomic>
#include <mutex>

template <typename T>
static void BM_RawNumberAdd(benchmark::State& state) {
  T counter = 0;

  for (auto _ : state) {
    // Adding loop will add ~1 ns overhead
    // for (int i = N; i > 0; --i)
    ++counter;
  }
  benchmark::DoNotOptimize(counter);
}

template <typename T>
static void BM_RawFencedNumberAdd(benchmark::State& state) {
  T counter = 0;

  for (auto _ : state) {
    benchmark::DoNotOptimize(++counter);
  }
  benchmark::DoNotOptimize(counter);
}

template <typename T>
static void BM_RawVolatileNumberAdd(benchmark::State& state) {
  volatile T counter = 0;

  for (auto _ : state) {
    ++counter;
  }
  benchmark::DoNotOptimize(counter);
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
  benchmark::DoNotOptimize(counter);
}

template <typename T>
static void BM_AtomicNumberAddST(benchmark::State& state) {
  std::atomic<T> counter(0);

  for (auto _ : state) {
    ++counter;
  }

  benchmark::DoNotOptimize(counter);
}

template <typename T>
static void BM_MutexGuardedNumberAddST(benchmark::State& state) {
  std::mutex lck;
  std::atomic<T> counter(0);

  for (auto _ : state) {
    std::lock_guard<std::mutex> guard(lck);
    ++counter;
  }

  benchmark::DoNotOptimize(counter);
}

BENCHMARK_TEMPLATE(BM_RawNumberAdd, int32_t);
BENCHMARK_TEMPLATE(BM_RawNumberAdd, float);
BENCHMARK_TEMPLATE(BM_RawNumberAddLooped, int32_t)->Arg(1);
BENCHMARK_TEMPLATE(BM_RawFencedNumberAdd, int32_t);
BENCHMARK_TEMPLATE(BM_RawVolatileNumberAdd, int32_t);
BENCHMARK_TEMPLATE(BM_AtomicNumberAddST, int32_t);
BENCHMARK_TEMPLATE(BM_AtomicNumberAddST, int64_t);
BENCHMARK_TEMPLATE(BM_MutexGuardedNumberAddST, int32_t);
