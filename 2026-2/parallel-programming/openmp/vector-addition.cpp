#include <iostream>
#include <cstdint>
#include <vector>
#include "hpc_helper.hpp"

using namespace std;

int main()
{
    const uint64_t num_entries = 1UL << 26;   // WSL 메모리 고려 (총 ~1.5GB)

    TIMERSTART(alloc)
    vector<no_init_t<uint64_t>> x(num_entries);
    vector<no_init_t<uint64_t>> y(num_entries);
    vector<no_init_t<uint64_t>> z(num_entries);
    TIMERSTOP(alloc)

    TIMERSTART(init)
    #pragma omp parallel for
    for (uint64_t i = 0; i < num_entries; i++) {
        x[i] = i;
        y[i] = num_entries - i;
        z[i] = 0;
    }
    TIMERSTOP(init)

    TIMERSTART(add_seq)
    for (uint64_t i = 0; i < num_entries; i++)
        z[i] = x[i] + y[i];
    TIMERSTOP(add_seq)

    TIMERSTART(add)
    #pragma omp parallel for
    for (uint64_t i = 0; i < num_entries; i++)
        z[i] = x[i] + y[i];
    TIMERSTOP(add)

    TIMERSTART(check)
    #pragma omp parallel for
    for (uint64_t i = 0; i < num_entries; i++)
        if (z[i] - num_entries)
            cout << "error at position " << i << endl;
    TIMERSTOP(check)
}