#include <cstring>
#include <cstdlib>

#include <memory>
#include <vector>
#include <iostream>

#include <benchmark/benchmark.h>

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 64
#endif

#ifndef CACHELINE_ALIGNMENT
#define CACHELINE_ALIGNMENT __attribute__((aligned(CACHELINE_SIZE)))
#endif

struct SharingStruct {
  int a;
  int b;
};

struct NoSharingStruct {
  int a;
  int pad[16];
  int CACHELINE_ALIGNMENT b;
};

class BM_Sharing: public benchmark::Fixture {
  public:
    SharingStruct CACHELINE_ALIGNMENT ss;
    NoSharingStruct CACHELINE_ALIGNMENT nss;
};

BENCHMARK_DEFINE_F(BM_Sharing, FalseSharing)(benchmark::State& state) {
  if (state.thread_index == 0) {
    for (auto _ : state) {
      benchmark::DoNotOptimize(++ss.a);
    }
  } else {
    for (auto _ : state) {
      benchmark::DoNotOptimize(++ss.b);
    }
  }
}

BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(2);

BENCHMARK_DEFINE_F(BM_Sharing, NoFalseSharing)(benchmark::State& state) {
  if (state.thread_index == 0) {
    for (auto _ : state) {
      benchmark::DoNotOptimize(++nss.a);
    }
  } else {
    for (auto _ : state) {
      benchmark::DoNotOptimize(++nss.b);
    }
  }
}

BENCHMARK_REGISTER_F(BM_Sharing, NoFalseSharing)->Threads(2);
