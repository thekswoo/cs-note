#include <iostream>
#include <vector>
#include <cstdint>
#include <thread>
#include <random>
#include <chrono>
#include <mutex>
#include <iomanip>

using namespace std;

#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
    a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::system_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label - a##label;      \
    std::cout << "#time (" << #label << "): "                     \
              << delta##label.count() << "s" << std::endl;

uint64_t single_thread_counter(const uint64_t& N)
{
    mt19937_64 engine(0);
    uniform_real_distribution<double> dist(0.0, 1.0);
    uint64_t count = 0;

    for (uint64_t i = 0; i < N; i++)
    {
        double x = dist(engine);
        double y = dist(engine);
        if (x * x + y * y <= 1.0)
        {
            count++;
        }
    }
    return count;
}

uint64_t mulit_thread_counter_local(const uint64_t& N, const uint64_t& num_threads)
{
    uint64_t global_count = 0;
    vector<thread> threads;
    vector<uint64_t> local_counts(num_threads, 0);
    
    auto multi_local = [&](const uint64_t &id)
    {
        uniform_real_distribution<double> dist(0.0, 1.0);
        mt19937_64 engine(id);
        uint64_t count = 0;
        for (uint64_t i = id; i < N; i += num_threads)
        {
            double x = dist(engine);
            double y = dist(engine);
            if (x * x + y * y <= 1.0)
            {
                count++;
            }
        }
        local_counts[id] = count;
    };

    for (uint64_t id = 0; id < num_threads; id++)
        threads.emplace_back(multi_local, id);
    for (auto &thread : threads)
        thread.join();
    for (auto &local_count : local_counts)
        global_count += local_count;

    return global_count;
}

uint64_t multi_thread_counter_global_no_mutex(const uint64_t& N, const uint64_t& num_threads)
{
    uint64_t global_count = 0;
    vector<thread> threads;
    
    auto multi_global_no_mutex = [&](const uint64_t &id)
    {
        uniform_real_distribution<double> dist(0.0, 1.0);
        mt19937_64 engine(id);
        for (uint64_t i = id; i < N; i += num_threads)
        {
            double x = dist(engine);
            double y = dist(engine);
            if (x * x + y * y <= 1.0)
            {
                global_count++;
            }
        }
    };

    for (uint64_t id = 0; id < num_threads; id++)
        threads.emplace_back(multi_global_no_mutex, id);
    for (auto &thread : threads)
        thread.join();
    return global_count;
}

uint64_t multi_thread_counter_global_yes_mutex(const uint64_t& N, const uint64_t& num_threads)
{
    uint64_t global_count = 0;
    vector<thread> threads;
    mutex m;
    
    auto multi_global_yes_mutex = [&](const uint64_t &id)
    {
        uniform_real_distribution<double> dist(0.0, 1.0);
        mt19937_64 engine(id);
        for (uint64_t i = id; i < N; i += num_threads)
        {
            double x = dist(engine);
            double y = dist(engine);
            if (x * x + y * y <= 1.0)
            {   lock_guard<mutex> lock_guard(m);
                global_count++;
            }
        }
    };

    for (uint64_t id = 0; id < num_threads; id++)
        threads.emplace_back(multi_global_yes_mutex, id);
    for (auto &thread : threads)
        thread.join();
    return global_count;
}

int main(int argc, char *argv[]) // argc : argument count(길이), argv : argument vector
{
    const uint64_t N = 100000000; // 10^8
    const double pi = 3.14159265358979323846;
    uint64_t num_threads[6] = {1, 2, 4, 8, 16, 32};   // 1 = single 전용
    const string labels[4] = {
        "single thread",
        "multi thread by local count",
        "multi thread by global count and no mutex",
        "multi thread by global count and yes mutex"
    };

    cout << fixed << setprecision(10);

    for (int mode = 0; mode < 4; mode++) {
        cout << "===========================================\n";
        cout << "[Result] " << labels[mode] << "\n\n";

        for (auto& num_thread : num_threads) {
            // single은 T=1만, 멀티스레드 구현은 T=1 제외
            if (mode == 0 && num_thread != 1) continue;
            if (mode != 0 && num_thread == 1) continue;
            
            uint64_t count = 0;

            cout << "Number of threads = " << num_thread << '\n';
            TIMERSTART(counter)
            switch (mode) {
                case 0: count = single_thread_counter(N); break;
                case 1: count = mulit_thread_counter_local(N, num_thread); break;
                case 2: count = multi_thread_counter_global_no_mutex(N, num_thread); break;
                case 3: count = multi_thread_counter_global_yes_mutex(N, num_thread); break;
            }
            TIMERSTOP(counter)

            double estimated_pi = 4.0 * count / N;
            double error = fabs(estimated_pi - pi);
            double error_rate = 100.0 * error / pi;

            cout << "#count        = " << count << '\n';
            cout << "#estimated pi = " << estimated_pi << '\n';
            cout << "#error        = " << error << '\n';
            cout << "#error rate   = " << error_rate << " %\n\n";
        }
    }
}