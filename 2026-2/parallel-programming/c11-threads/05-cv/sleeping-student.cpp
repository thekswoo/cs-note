#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
using namespace std;

int main(){
    mutex m;
    condition_variable cv;
    bool time_for_breakfast = false;

    auto student = [&]() -> void {
        {
            unique_lock<mutex> unique_lock(m);
            while(!time_for_breakfast) {
                cv.wait(unique_lock);
            }
        }

        cout << "Time to make some coffee!" << endl;
    };

    thread my_thread(student);
    this_thread::sleep_for(4s);
    {
        lock_guard<mutex> lock_guard(m);
        time_for_breakfast = true;
    }
    cv.notify_one();
    my_thread.join();
}