#pragma once

#include <concepts>

template <typename T>
concept Interchange = requires(T a, const T b, const T c, const float f) {
    T();

    { a.init() } -> std::same_as<void>;

    { b + c } -> std::same_as<T>;
    { b - c } -> std::same_as<T>;
    { b * c } -> std::same_as<T>;
    { b / c } -> std::same_as<T>;

    { a += b } -> std::same_as<void>;
    { a -= b } -> std::same_as<void>;
    { a *= b } -> std::same_as<void>;
    { a /= b } -> std::same_as<void>;

    { a += f } -> std::same_as<void>;
    { a -= f } -> std::same_as<void>;
    { a *= f } -> std::same_as<void>;
    { a /= f } -> std::same_as<void>;
};