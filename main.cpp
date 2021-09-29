#include <iostream>
#include <chrono>
#include <thread>
#include <new>
#include <array>
#include <memory>
#include <random>

#include <benchmark/benchmark.h>
#include <boost/random/mersenne_twister.hpp>

#include "spsc_queue.h"
#include "simple_queue.h"

/*
 * DO NOTHING TEST
 */
void BM_empty(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(state.iterations());
    }
}
BENCHMARK(BM_empty);
BENCHMARK(BM_empty)->ThreadPerCpu();

/*
 * RANDOM TEST
 */
void BM_mt19937(benchmark::State& state) {
    boost::random::mt19937 gen;
    for(auto _ : state) {
        benchmark::DoNotOptimize(gen());
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(unsigned int));
}
BENCHMARK(BM_mt19937);

void BM_stdmt19937(benchmark::State& state) {
    std::mt19937 gen;
    for(auto _ : state) {
        benchmark::DoNotOptimize(gen());
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(int));
}
BENCHMARK(BM_stdmt19937);

/*
 * SPSC QUEUE
 */

void BM_spsc_basic(benchmark::State& state) {
    deaod::spsc_queue<int, 32> queue;
    bool pushed;
    int out;
    int counter = 0;
    for (auto _ : state) {
        if(queue.push(counter++))
            benchmark::DoNotOptimize(queue.pop(out));
    }
    state.SetBytesProcessed(state.iterations() * sizeof(int));
}
BENCHMARK(BM_spsc_basic);

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
