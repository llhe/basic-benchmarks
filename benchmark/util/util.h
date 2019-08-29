#ifndef BENCHMARK_UTIL_UTIL_H_
#define BENCHMARK_UTIL_UTIL_H_

template <class T>
void DoNotOptimizeAway(T&& datum) {
    asm volatile("" : "+r" (datum));
}

#endif  // BENCHMARK_UTIL_UTIL_H_
