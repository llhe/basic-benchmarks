#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>

#include <benchmark/benchmark.h>

static void BM_BinarySearch(benchmark::State& state) {
  int n = state.range(0);
  std::vector<int> v;
  for (int i = 0; i < n; ++i) {
    if (i % 2 == 0) {
      v.push_back(i);
    }
  }

  int idx = 0;
  for (auto _ : state) {
    std::binary_search(v.begin(), v.end(), ++idx % n);
  }
}

template <typename Set>
static void BM_SetSearch(benchmark::State& state) {
  int n = state.range(0);

  Set s;
  for (int i = 0; i < n; ++i) {
    if (i % 2 == 0) {
      s.insert(i);
    }
  }

  int idx = 0;
  for (auto _ : state) {
    s.find(++idx % n);
  }
}

BENCHMARK(BM_BinarySearch)->RangeMultiplier(4)->Range(4, 4096);
BENCHMARK_TEMPLATE(BM_SetSearch, std::set<int>)->RangeMultiplier(4)
                                               ->Range(4, 4096);
BENCHMARK_TEMPLATE(BM_SetSearch, std::unordered_set<int>)->RangeMultiplier(4)
                                                         ->Range(4, 4096);
