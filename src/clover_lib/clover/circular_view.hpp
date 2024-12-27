#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"
#include <iostream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <vector>

namespace clover {

template <typename T>
concept circular_viewable = requires {
    typename T::iterator_category;
    typename T::value_type;
    typename T::difference_type;
    typename T::pointer;
    typename T::reference;

    requires std::contiguous_iterator<T>;
};

template <circular_viewable T>
struct circular_view : std::ranges::view_interface<circular_view<T>> {
    struct iterator {
        using iterator_category = typename T::iterator_category;
        using value_type        = typename T::value_type;
        using difference_type   = typename T::difference_type;
        using pointer           = typename T::pointer;
        using reference         = typename T::reference;

        const T m_begin;
        const T m_sentinel;
        T m_current;
        bool m_has_moved = false;

        value_type operator*();
        reference operator*() const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator+=(difference_type n);
        iterator operator+(difference_type n) const;
        iterator& operator--();
        iterator operator--(int);
        iterator& operator-=(difference_type n);
        iterator operator-(difference_type n) const;
        bool operator==(const iterator& other);
        bool operator!=(const iterator& other);
    };

    const T m_begin;
    const T m_sentinel;
    const T m_begin_from;

    circular_view(T start, T sentinel);
    circular_view(T start, T sentinel, T start_from);
    circular_view(T start, T sentinel, iterator start_from);
    iterator begin() const;
    iterator end() const;
    circular_view from(iterator it) const;
    circular_view from(T it) const;
    circular_view from(iterator::difference_type value) const;
    size_t size();
};

}  // namespace clover

// int main(int argc, char* argv[]) {
//     std::vector<float> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     clover::circular_view circ{vec.begin(), vec.end()};

//     auto iter = circ.begin();

//     std::cout << "start iota" << std::endl;

//     for (auto x : std::views::iota(0, 20)) {
//         std::cout << (*iter) << "  " << std::endl;
//         iter++;
//     }

//     std::cout << "done iota" << std::endl;
//     std::cout << "start default range" << std::endl;

//     for (auto& x : circ) {
//         x = x * 2;
//     }

//     for (auto& x : circ) {
//         std::cout << x << std::endl;
//     }

//     for (auto& x : circ) {
//         x = x / 2;
//     }

//     std::cout << "done default range" << std::endl;

//     std::cout << "start custom range" << std::endl;

//     for (auto& x : circ.from(-2)) {
//         std::cout << x << std::endl;
//     }

//     std::cout << "done custom range" << std::endl;
//     std::cout << "size: " << circ.size() << std::endl;
// }
