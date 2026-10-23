#include <iostream>
#include <mutex>
#include <vector>

template <typename index_t, typename value_t>
void dynamic_all_pairs(vector<value_t> &mnist, vector<value_t> &all_pair,
                       index_t rows, index_t cols, index_t num_threads = 64, index_t chunk_size = 64 / sizeof(value_t))
{
    mutex m;
    incdex_t global_lower = 0;
    auto dynamic_block_cyclic = [&]() -> void
    {
        index_t lower = 0;

        while (lower < rows)
        {
            lock_guard<mutex> lock_guard(m);
            lower = global_lower;
            global_lower += chunk_size;
        }
        
        const index_t upper = min(lower + chunk_size, rows);

        for(index_t i = lower; i<upper; i++) {
            for(index_t I = 0; I <= i; I++) {
                value_t accum = value_t(0);
                for(index_t j = 0; j<cols; j++) {
                    value_t residue = mnist[i*cols + j] - mnist[I*cols + j];
                    accum += residue * residue;
                }
                all_pair[i*rows + I] = all_pair[I*rows + i] = accum;
            }
        }
    };
};