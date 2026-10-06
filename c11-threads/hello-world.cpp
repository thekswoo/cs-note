#include <cstdint>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

void say_hello(uint64_t id) {
    cout << "Hello from thread: " << id << endl;
}

int main(int argc, char* argv[]) {
    const uint64_t num_threads = 4;
    vector<thread> threads;

    for(uint64_t id = 0; id < num_threads; id++) {
        threads.emplace_back(say_hello, id);
    }

    for(auto& thread : threads) {
        thread.join(); // 모든 쓰레드가 끝날때까지 main이 대기
        // thread.detach(); // 안 기다리고 바로 진행
    }
}
