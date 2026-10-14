#pragma once          

#include <chrono>     
#include <iostream>   
#include <type_traits>

#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
    a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::system_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label - a##label;      \
    std::cout << "#time (" << #label << "): "                     \
              << delta##label.count() << "s" << std::endl;


template <class T>
class no_init_t {
public:
    static_assert(std::is_fundamental<T>::value && std::is_arithmetic<T>::value,
                  "wrapped type must be a fundamental, numeric type");

    constexpr no_init_t() noexcept {}                  // 핵심: 아무것도 안 함
    constexpr no_init_t(T value) noexcept : v_(value) {} // T → no_init_t 변환
    constexpr operator T() const noexcept { return v_; } // no_init_t → T 변환

private:
    T v_;
};