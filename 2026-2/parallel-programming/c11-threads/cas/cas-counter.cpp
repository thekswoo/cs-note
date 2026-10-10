#include <iostream>
#include <vector>
#include <thread>
#include <cstdint>
#include <atomic>

using namespace std;

#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
    a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                              \
    b##label = std::chrono::system_clock::now();                      \
    std::chrono::duration<double> delta##label = b##label - a##label; \
    std::cout << "# elapsed time (" << #label << "): "                \
              << delta##label.count() << "s" << std::endl;


int main() {
    vector<thread> threads;
    const uint64_t num_threads = 10;
    const uint64_t num_iters = 1000000000;

    auto false_max = [&](volatile atomic<uint64_t>* counter, const auto& id) -> void {
        for(uint64_t i = id; i < num_iters; i += num_threads) {
            if(i > *counter) {
                *counter = i;
            }
        }
    };

    auto correct_max = [&](volatile atomic<uint64_t>* counter, const auto& id) -> void {
        for(uint64_t i = id; i < num_iters; i += num_threads){
            auto previous = counter->load();
            while(previous < i && !counter->compare_exchange_weak(previous, i)) {}
        }
    };

    TIMERSTART(incorrect_max)
    atomic<uint64_t> false_counter(0);
    threads.clear();
    for(uint64_t id = 0; id < num_threads; id++) {
        threads.emplace_back(false_max, &false_counter, id);
    }
    for(auto& thread : threads){
        thread.join();
    }
    TIMERSTOP(incorrect_max)


    TIMERSTART(correct_max)
    atomic<uint64_t> correct_counter(0);
    threads.clear();
    for(uint64_t id = 0; id < num_threads; id++) {
        threads.emplace_back(correct_max, &correct_counter, id);
    }
    for(auto& thread : threads){
        thread.join();
    }
    TIMERSTOP(correct_max)

    cout << false_counter << " " << correct_counter << '\n';
}