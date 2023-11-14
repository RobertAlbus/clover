#pragma once

#include <concepts>
// #include <type_traits> // included by <concepts>


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

