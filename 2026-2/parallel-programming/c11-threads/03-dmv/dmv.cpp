#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;
#define SDIV(x, y) (((x) + (y) - 1) / (y))
template <typename value_t, typename index_t>
void init(vector<value_t> &A, vector<value_t> &x, index_t m, index_t n)
{
    for (index_t row = 0; row < m; row++)
    {
        for (index_t col = 0; col < n; col++)
        {
            A[row * n + col] = row >= col ? 1 : 0; // 하삼각행렬
        }
    }

    for (index_t col = 0; col < n; col++)
    {
        x[col] = col;
    }
}

template <typename value_t, typename index_t>
void sequential(vector<value_t> &A, vector<value_t> &x, vector<value_t> &B, index_t m, index_t n)
{
    for (index_t row = 0; row < m; row++)
    {
        value_t accum = 0;
        for (index_t col = 0; col < n; col++)
        {
            accum += A[row * n + col] * x[col];
        }
        B[row] = accum;
    }
}

template <typename value_t, typename index_t>
void block_parallel_mult(vector<value_t> &A, vector<value_t> &x, vector<value_t> &B,
                         index_t m, index_t n, index_t num_threads = 8)
{
    auto block = [&](const index_t &id) -> void
    {
        const index_t chunck = SDIV(m, num_threads);  // 중요
        const index_t lower = chunck * id;            // 중요
        const index_t upper = min(lower + chunck, m); // 중요

        for (index_t row = lower; row < upper; row++)
        {
            value_t accum = value_t(0); // accumulator 누적으로 쌓이는 변수
            for (index_t col = 0; col < n; col++)
            {
                accum += A[row * n + col] * x[col];
            }
            B[row] = accum;
        }
    };

    vector<thread> threads;
    for (index_t id = 0; id < num_threads; id++)
    {
        threads.emplace_back(block, id);
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
}

template <typename value_t, typename index_t>
void cyclic_parallel_mult(vector<value_t> &A, vector<value_t> &x, vector<value_t> &B,
                          index_t m, index_t n, index_t num_threads = 8)
{
    auto cyclic = [&](const index_t &id) -> void
    {
        for (index_t row = id; row < m; row += num_threads)
        {
            value_t accum = value_t(0);
            for (index_t col = 0; col < n; col++)
            {
                accum += A[row * n + col] * x[col];
            }
            B[row] = accum;
        }
    };

    vector<thread> threads;
    for (index_t id = 0; id < num_threads; id++)
        threads.emplace_back(cyclic, id);
    for (auto &thread : threads)
        thread.join();
}

template <typename value_t, typename index_t>
void block_cyclic_parallel_mul(vector<value_t> &A, vector<value_t> &x,
                               vector<value_t> &B, index_t m, index_t n, index_t num_threads = 8,
                               index_t chunk_size = 64 / sizeof(value_t))
{
    auto block_cyclic = [&](const index_t &id) -> void
    {
        const index_t offset = id * chunk_size;          // 중요 : 각 thread별 시작점
        const index_t stride = num_threads * chunk_size; // 중요 : 각 thread가 다음 cycle에 받을 시작점

        for (index_t lower = offset; lower < m; lower += stride)
        {
            const index_t upper = min(lower + chunk_size, m);
            for (index_t row = lower; row < upper; row++)
            {
                value_t accum = value_t(0);
                for (index_t col = 0; col < n; col++)
                {
                    accum += A[row * n + col] * x[col];
                }
                B[row] = accum;
            }
        }
    };

    vector<thread> threads;
    for(index_t id = 0; id<num_threads; id++) threads.emplace_back(block_cyclic, id);
    for(auto& thread : threads) thread.join();
}

int main()
{
    const uint64_t n = 1UL << 15;
    const uint64_t m = 1UL << 15;

    vector<uint64_t> A(m * n);
    vector<uint64_t> x(n);
    vector<uint64_t> B(m);

    init(A, x, m, n);

    sequential(A, x, B, m, n);
    block_parallel_mult(A, x, B, m, n);
    cyclic_parallel_mult(A, x, B, m, n);
}