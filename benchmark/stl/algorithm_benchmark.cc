#include <algorithm>
#include <vector>
#include <random>

#include <benchmark/benchmark.h>

template <typename Generator, typename Distribution>
static void BM_RandGen(benchmark::State& state) {
  Generator generator;
  Distribution dist;
  
  for (auto _ : state) {
    dist(generator);
  }
}

static void BM_StdSort(benchmark::State& state) {
  int n = state.range(0);
  std::vector<int> v(n);
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist;
  
  for (auto _ : state) {
    state.PauseTiming();
    std::generate(v.begin(), v.end(), [&] () { return dist(generator); });
    state.ResumeTiming();

    std::sort(v.begin(), v.end());
  }
}

BENCHMARK_TEMPLATE(BM_RandGen,
    std::default_random_engine,
    std::uniform_int_distribution<int>);
BENCHMARK_TEMPLATE(BM_RandGen,
    std::mt19937,
    std::uniform_int_distribution<int>);
BENCHMARK_TEMPLATE(BM_RandGen,
    std::mt19937,
    std::normal_distribution<double>);
BENCHMARK(BM_StdSort)->RangeMultiplier(4)->Range(4, 1024);
