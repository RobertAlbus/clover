#pragma once

#include <concepts>

template <typename T>
concept InterchangeArithmetic = requires(T a, const T b, const float f) {
    { a.init() } -> std::same_as<void>;

    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;

    { a += b } -> std::same_as<void>;
    { a -= b } -> std::same_as<void>;
    { a *= b } -> std::same_as<void>;
    { a /= b } -> std::same_as<void>;

    { a += f } -> std::same_as<void>;
    { a -= f } -> std::same_as<void>;
    { a *= f } -> std::same_as<void>;
    { a /= f } -> std::same_as<void>;
};