#pragma once

#include <chrono>
#include <concepts>
#include <cstdio>
#include <functional>
#include <latch>
#include <map>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <vector>


struct FrameBase {
    FrameBase() = default;
};

template <typename T>
concept Frame = 
    std::is_base_of_v<FrameBase, T> 
    && requires(T a, const T b, const T& c) {
        T();
        { a.clone() } -> std::same_as<T>;
        { b + c } -> std::same_as<T>;
        { a += b } -> std::same_as<T&>;
        { a.reset() } -> std::same_as<void>;
        
        // { b * c } -> std::same_as<T>;
        // { a *= b } -> std::same_as<void>;
};

