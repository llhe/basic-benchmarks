#include <cstring>
#include <cstdlib>

#include <memory>
#include <vector>
#include <iostream>

#include <benchmark/benchmark.h>

class BM_Memory: public benchmark::Fixture {
  public:
    std::unique_ptr<char> src;
    std::unique_ptr<char> dst;

    void SetUp(const ::benchmark::State& state) {
      if (state.thread_index == 0) {
        int bytes = state.range(0);
        src = std::unique_ptr<char>(new char[bytes]);
        dst = std::unique_ptr<char>(new char[bytes]);
      }
    }

    void TearDown(const ::benchmark::State& state) {
    }
};

BENCHMARK_DEFINE_F(BM_Memory, Memset)(benchmark::State& state) {
  int block_size = state.range(0) / state.threads;
  int ti = state.thread_index;

  for (auto _ : state) {
    void *dst_ptr = src.get() + ti * block_size;
    memset(dst_ptr, '0', block_size);
  }
  state.SetBytesProcessed(int64_t(state.iterations()) * block_size);
}

BENCHMARK_REGISTER_F(BM_Memory, Memset)
  ->RangeMultiplier(8)->Range(8, 1 << 28);
BENCHMARK_REGISTER_F(BM_Memory, Memset)
  ->UseRealTime()->Arg(1 << 28)->Threads(2);
BENCHMARK_REGISTER_F(BM_Memory, Memset)
  ->UseRealTime()->Arg(1 << 28)->Threads(4);
BENCHMARK_REGISTER_F(BM_Memory, Memset)
  ->UseRealTime()->Arg(1 << 28)->Threads(8);


BENCHMARK_DEFINE_F(BM_Memory, Memcpy)(benchmark::State& state) {
  int block_size = state.range(0) / state.threads;
  int ti = state.thread_index;

  for (auto _ : state) {
    void *src_ptr = src.get() + ti * block_size;
    void *dst_ptr = dst.get() + ti * block_size;
    memcpy(dst_ptr, src_ptr, block_size);
  }
  state.SetBytesProcessed(int64_t(state.iterations()) * block_size);
}

BENCHMARK_REGISTER_F(BM_Memory, Memcpy)
  ->RangeMultiplier(8)->Range(8, 1 << 28);
BENCHMARK_REGISTER_F(BM_Memory, Memcpy)
  ->UseRealTime()->Arg(1 << 28)->Threads(2);
BENCHMARK_REGISTER_F(BM_Memory, Memcpy)
  ->UseRealTime()->Arg(1 << 28)->Threads(4);
BENCHMARK_REGISTER_F(BM_Memory, Memcpy)
  ->UseRealTime()->Arg(1 << 28)->Threads(8);
