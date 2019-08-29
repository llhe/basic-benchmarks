#include <cstring>
#include <string>

#include <benchmark/benchmark.h>

static void BM_StringCreationEmpty(benchmark::State& state) {
	for (auto _ : state) {
		std::string empty_string;
		benchmark::DoNotOptimize(empty_string);
	}
}
BENCHMARK(BM_StringCreationEmpty);

static void BM_StringCreation(benchmark::State& state) {
	for (auto _ : state) {
		std::string x("hello");
		benchmark::DoNotOptimize(x);
	}
}
BENCHMARK(BM_StringCreation);

static void BM_StringCopy(benchmark::State& state) {
	std::string x = "hello";
	for (auto _ : state) {
		std::string copy(x);
		// copy[0] = 'a'; // no extra time, no copy on write?
		benchmark::DoNotOptimize(copy);
	}
}

BENCHMARK(BM_StringCopy);

static void BM_StringCopyC(benchmark::State& state) {
	std::string x = "hello";
	for (auto _ : state) {
    char copy[10];
    strncpy(copy, x.c_str(), x.size());
		benchmark::DoNotOptimize(copy);
	}
}

BENCHMARK(BM_StringCopyC);

static void BM_StringConcat(benchmark::State& state) {
	std::string x = "hello";
	std::string y = "world";
	for (auto _ : state) {
		std::string z = x + y;
		benchmark::DoNotOptimize(z);
	}
}

BENCHMARK(BM_StringConcat);

static void BM_StringConcatC(benchmark::State& state) {
	std::string x = "hello";
	std::string y = "world";
	for (auto _ : state) {
		char z[20];
    strncpy(z, x.c_str(), x.size());
    strncpy(z + x.size(), y.c_str(), y.size());
		benchmark::DoNotOptimize(z);
	}
}

BENCHMARK(BM_StringConcatC);
