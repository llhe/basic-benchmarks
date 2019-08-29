#ifndef BENCHMARK_UTILS_H_
#define BENCHMARK_UTILS_H_

template <class T>
void DoNotOptimizeAway(T&& datum) {
    asm volatile("" : "+r" (datum));
}

#endif  // BENCHMARK_UTILS_H_
