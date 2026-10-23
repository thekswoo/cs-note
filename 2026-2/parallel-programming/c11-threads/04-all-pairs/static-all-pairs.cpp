#include <iostream>  // std::cout
#include <cstdint>   // uint64_t
#include <vector>    // std::vector
#include <thread>    // std::thread
#include <algorithm> // std::min
#include <fstream>   // std::ifstream
#include <random>    // 파일 없을 때 랜덤 데이터
#include <chrono>    // 시간 측정
#include <cmath>     // std::abs

// ----------------------------------------------------------------------------
// 헬퍼 (강의자료에서는 정의가 생략된 부분)
// ----------------------------------------------------------------------------

// vector<T>(N) 생성 시 0으로 초기화하는 비용을 없애기 위한 래퍼
template <typename T>
struct no_init_t
{
    T value;
    no_init_t() {} // 기본 생성자가 아무것도 안 함 → 초기화 생략
    no_init_t(T v) : value(v) {}
    operator T &() { return value; }
    operator const T &() const { return value; }
};

// 바이너리 파일을 배열에 그대로 읽어오기 (성공 여부 반환)
template <typename value_t, typename index_t>
bool load_binary(value_t *data, index_t length, const char *filename)
{
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    if (!ifs)
        return false;
    ifs.read(reinterpret_cast<char *>(data), sizeof(value_t) * length);
    return static_cast<bool>(ifs);
}

// ----------------------------------------------------------------------------
// Sequential computation of All-Pairs Distance Matrix
// ----------------------------------------------------------------------------
template <typename index_t, typename value_t>
void sequential_all_pairs(std::vector<value_t> &mnist, std::vector<value_t> &all_pair,
                          index_t rows, index_t cols)
{
    for (index_t i = 0; i < rows; i++) // for all entries below the diagonal (i' = I)
    {
        for (index_t I = 0; I <= i; I++)
        {
            value_t accum = value_t(0); // compute squared Euclidean distance
            for (index_t j = 0; j < cols; j++)
            {
                value_t residue = mnist[i * cols + j] - mnist[I * cols + j];
                accum += residue * residue;
            }
            all_pair[i * rows + I] = all_pair[I * rows + i] = accum; // Delta[i,i'] = Delta[i',i] = dist(i,i')
        }
    }
}

// ----------------------------------------------------------------------------
// All-pairs matrix template using a static block-cyclic distribution
// ----------------------------------------------------------------------------
template <typename index_t, typename value_t>
void parallel_all_pairs(std::vector<value_t> &mnist, std::vector<value_t> &all_pair,
                        index_t rows, index_t cols, index_t num_threads = 64,
                        index_t chunk_size = 64 / sizeof(value_t))
{
    auto block_cyclic = [&](const index_t &id) -> void
    {
        const index_t off = id * chunk_size;          // precompute offset and stride
        const index_t str = num_threads * chunk_size;

        for (index_t lower = off; lower < rows; lower += str) // for each block of chunk_size in cyclic order
        {
            const index_t upper = std::min(lower + chunk_size, rows); // compute upper border of block

            for (index_t i = lower; i < upper; i++) // for all entries below the diagonal (i' = I)
            {
                for (index_t I = 0; I <= i; I++)
                {
                    value_t accum = value_t(0); // compute squared Euclidean distance
                    for (index_t j = 0; j < cols; j++)
                    {
                        value_t residue = mnist[i * cols + j] - mnist[I * cols + j];
                        accum += residue * residue;
                    }
                    all_pair[i * rows + I] = all_pair[I * rows + i] = accum; // Delta[i,i'] = Delta[i',i]
                }
            }
        }
    };

    std::vector<std::thread> threads; // business as usual
    for (index_t id = 0; id < num_threads; id++)
        threads.emplace_back(block_cyclic, id);
    for (auto &thread : threads)
        thread.join();
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
int main()
{
    typedef no_init_t<float> value_t; // used data types
    typedef uint64_t index_t;

    // 강의자료는 rows = 65000이지만, all_pair가 rows^2 * 4B라 메모리가 엄청 큼
    //   rows = 60000 → 약 14GB / rows = 2000 → 약 16MB
    // 먼저 작은 값으로 테스트하고 늘리기
    const index_t rows = 2000;    // number of images (최대 60000)
    const index_t cols = 28 * 28; // number of pixels

    std::vector<value_t> mnist(rows * cols);
    if (!load_binary(mnist.data(), rows * cols, "./data/mnist_60000_28_28_32.bin"))
    {
        std::cout << "MNIST 파일 없음 → 랜덤 데이터 사용\n";
        std::mt19937 gen(42);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        for (auto &v : mnist)
            v = dist(gen);
    }

    std::vector<value_t> seq_result(rows * rows);
    std::vector<value_t> par_result(rows * rows);

    // 1) sequential
    auto t0 = std::chrono::high_resolution_clock::now();
    sequential_all_pairs(mnist, seq_result, rows, cols);
    auto t1 = std::chrono::high_resolution_clock::now();

    // 2) static block-cyclic
    const index_t num_threads = std::thread::hardware_concurrency(); // 내 CPU 논리
    parallel_all_pairs(mnist, par_result, rows, cols); // 코어 수
    const index_t chunk_size = 64 / sizeof(value_t);                 // float → 16
    auto t2 = std::chrono::high_resolution_clock::now();

    double seq_sec = std::chrono::duration<double>(t1 - t0).count();
    double par_sec = std::chrono::duration<double>(t2 - t1).count();

    // 결과 검증
    bool ok = true;
    for (index_t k = 0; k < rows * rows; k++)
    {
        if (std::abs(float(seq_result[k]) - float(par_result[k])) > 1e-3f)
        {
            ok = false;
            break;
        }
    }

    std::cout << "rows = " << rows << ", threads = " << num_threads
              << ", chunk = " << chunk_size << "\n";
    std::cout << "sequential   : " << seq_sec << " s\n";
    std::cout << "block-cyclic : " << par_sec << " s\n";
    std::cout << "speedup      : " << seq_sec / par_sec << "\n";
    std::cout << "result check : " << (ok ? "OK" : "MISMATCH") << "\n";
}