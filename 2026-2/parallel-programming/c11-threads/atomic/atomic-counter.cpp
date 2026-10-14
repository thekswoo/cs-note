#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include "hpc_helper.hpp"
using namespace std;

int main()
{
    mutex m;
    vector<thread> threads;
    const uint64_t num_threads = 10;
    const uint64_t num_iters = 100000000;

    auto lock_count = [&](volatile uint64_t *counter, const auto &id) -> void
    {
        for (uint64_t i = id; i < num_iters; i += num_threads)
        {
            lock_guard<mutex> lock_guard(m);
            (*counter)++;
        }
    };

    auto atomic_count = [&](volatile atomic<uint64_t> *counter, const auto &id) -> void
    {
        for (uint64_t i = id; i < num_iters; i += num_threads)
        {
            (*counter)++;
        }
    };

    TIMERSTART(mutex_multithreaded)
    uint64_t counter = 0;
    threads.clear();
    for (uint64_t id = 0; id < num_threads; id++)
    {
        threads.emplace_back(lock_count, &counter, id);
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    TIMERSTOP(mutex_multithreaded)
    
    TIMERSTART(atomic_multithreaded)
    atomic<uint64_t> atomic_counter(0);
    threads.clear();
    for (uint64_t id = 0; id < num_threads; id++)
    {
        threads.emplace_back(atomic_count, &atomic_counter, id);
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    TIMERSTOP(atomic_multithreaded)

    cout << counter << " " << atomic_counter << '\n';
}