#include <iostream>
#include <chrono>
#include <thread>
#include <new>
#include <array>
#include <memory>

#include <benchmark/benchmark.h>
#include "spsc_queue.h"
#include "simple_queue.h"

//#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

void BM_empty(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(state.iterations());
    }
}
BENCHMARK(BM_empty);
BENCHMARK(BM_empty)->ThreadPerCpu();
//BENCHMARK_MAIN();


int main(int argc, char** argv) {

    std::cout << "Hello, World!" << std::endl;
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        std::cout << "ReportUnrecognizedArguments" << std::endl;
        return 1;
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}
