#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

int main()
{
    mutex m;
    condition_variable cv;
    bool is_ping = true;

    auto ping = [&]() -> void
    {
        while (true)
        {
            unique_lock<mutex> lock(m);
            cv.wait(lock, [&]() { return is_ping; });   // 내 차례(true)일 때까지 잠듦
            this_thread::sleep_for(1s);
            cout << "ping" << endl;
            is_ping = !is_ping;                          // 공 넘기기
            cv.notify_one();                             // 상대 깨우기
        }                                                // scope 끝 → unlock
    };

    auto pong = [&]() -> void
    {
        while (true)
        {
            unique_lock<mutex> lock(m);
            cv.wait(lock, [&]() { return !is_ping; });  // 내 차례(false)일 때까지 잠듦
            this_thread::sleep_for(1s);
            cout << "pong" << endl;
            is_ping = !is_ping;
            cv.notify_one();
        }
    };

    thread ping_thread(ping);
    thread pong_thread(pong);
    ping_thread.join();
    pong_thread.join();
}