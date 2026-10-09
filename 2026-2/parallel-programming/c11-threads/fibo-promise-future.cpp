#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <future>

using namespace std;

template <typename value_t, typename index_t>

void fibo(value_t n, promise<value_t> &&result)
{ // promise객체를 받음
    value_t a_0 = 0, a_1 = 1;
    for (index_t index = 0; index < n; index++)
    {
        const value_t temp = a_0;
        a_0 = a_1;
        a_1 = a_1 + temp;
    }
    result.set_value(a_0); // 넘겨받은 promise객체에 값 저장 -> main thread에서 future.get()호출했을 때 block 해제
}

int main(int argc, char *argv[])
{
    const uint64_t num_thread = 32;
    vector<thread> threads;
    vector<future<uint64_t>> results;

    for (uint64_t id = 0; id < num_thread; id++)
    {
        promise<uint64_t> promise;                                         // promise 객체 생성
        results.emplace_back(promise.get_future());                        // future객체 생성
        threads.emplace_back(fibo<uint64_t, uint64_t>, id, move(promise)); // promise객체를 fibo에 넘김
    }

    // 메인쓰레드에서 result에 담긴
    for (auto &result : results)
        cout << result.get() << endl; // synchronize point
    for (auto &thread : threads)
        thread.join();
}