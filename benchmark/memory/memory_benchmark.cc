#include <cstring>
#include <cstdlib>

#include <memory>
#include <vector>

#include <benchmark/benchmark.h>

static void BM_Memcpy(benchmark::State& state) {
  int bytes = state.range(0);
  auto src = std::unique_ptr<char>(new char[bytes]);
  auto dst = std::unique_ptr<char>(new char[bytes]);

  // warm-ups
  memcpy(dst.get(), src.get(), bytes);

  for (auto _ : state) {
    memcpy(dst.get(), src.get(), bytes);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * bytes);
  // state.counters["throughput"] = benchmark::Counter(
  //     static_cast<int64_t>(state.iterations()) * bytes,
  //       benchmark::Counter::kIsRate,
  //       benchmark::Counter::OneK::kIs1024);
}

BENCHMARK(BM_Memcpy)->RangeMultiplier(8)->Range(8, 1 << 28);

BENCHMARK(BM_Memcpy)->UseRealTime()->Arg(1 << 28)->Threads(2);
BENCHMARK(BM_Memcpy)->UseRealTime()->Arg(1 << 28)->Threads(4);
BENCHMARK(BM_Memcpy)->UseRealTime()->Arg(1 << 28)->Threads(8);
