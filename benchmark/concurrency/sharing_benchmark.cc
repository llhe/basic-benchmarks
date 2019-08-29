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

struct CACHELINE_ALIGNMENT SharingStruct {
  int a;
  int b;
};

struct CACHELINE_ALIGNMENT NoSharingStruct {
  int a;
  int CACHELINE_ALIGNMENT b;
};

class BM_Sharing: public benchmark::Fixture {
  public:
    SharingStruct ss;
    NoSharingStruct nss;

    void SetUp(const ::benchmark::State& state) {}
    void TearDown(const ::benchmark::State& state) {}
};

BENCHMARK_DEFINE_F(BM_Sharing, FalseSharing)(benchmark::State& state) {
  int *ptr = nullptr;
  if (state.thread_index == 0) {
    ptr = &ss.a;
  } else {
    ptr = &ss.b;
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(++(*ptr));
  }
}

BENCHMARK_REGISTER_F(BM_Sharing, FalseSharing)->Threads(2);

BENCHMARK_DEFINE_F(BM_Sharing, NoFalseSharing)(benchmark::State& state) {
  int *ptr = nullptr;
  if (state.thread_index == 0) {
    ptr = &nss.a;
  } else {
    ptr = &nss.b;
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(++(*ptr));
  }
}

BENCHMARK_REGISTER_F(BM_Sharing, NoFalseSharing)->Threads(2);
