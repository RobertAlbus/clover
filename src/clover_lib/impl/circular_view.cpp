// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/circular_view.hpp"

namespace clover {

template <circular_viewable T>
T::value_type circular_view<T>::iterator::operator*() {
    return *m_current;
}

template <circular_viewable T>
T::reference circular_view<T>::iterator::operator*() const {
    return *m_current;
}

template <circular_viewable T>
circular_view<T>::iterator& circular_view<T>::iterator::operator++() {
    ++m_current;
    m_has_moved = true;
    if (m_current == m_sentinel) {
        std::cout << "HIT SENTINEL" << std::endl;
        m_current = m_begin;
    }

    return *this;
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::iterator::operator++(int) {
    iterator result = *this;
    ++(*this);
    return result;
}

template <circular_viewable T>
circular_view<T>::iterator& circular_view<T>::iterator::operator+=(difference_type n) {
    difference_type size = std::distance(m_begin, m_sentinel);

    if (n >= size) {
        n = n % size;
    } else if (n < 0) {
        n = (n % size) + size;
    }

    size_t remaining_before_wrap = std::distance(m_current, m_sentinel - 1);

    if (n <= remaining_before_wrap) {
        m_current += n;
        return *this;
    }

    m_current = m_begin + (size - n);
    return *this;
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::iterator::operator+(difference_type n) const {
    iterator result    = *this;
    result.m_has_moved = false;
    result += n;

    return result;
}

template <circular_viewable T>
circular_view<T>::iterator& circular_view<T>::iterator::operator--() {
    if (m_current != m_begin)
        --m_current;
    else
        m_current = m_sentinel - 1;

    return *this;
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::iterator::operator--(int) {
    iterator result = *this;
    --(*this);
    return result;
}

template <circular_viewable T>
circular_view<T>::iterator& circular_view<T>::iterator::operator-=(difference_type n) {
    return *this += -n;
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::iterator::operator-(difference_type n) const {
    return *this + -n;
}

template <circular_viewable T>
bool circular_view<T>::iterator::operator==(const iterator& other) {
    bool is_equal = m_current == other.m_current && m_has_moved;
    if (is_equal) {
        m_has_moved = false;
    }
    return is_equal;
}

template <circular_viewable T>
bool circular_view<T>::iterator::operator!=(const iterator& other) {
    return !(*this == other);
}

template <circular_viewable T>
circular_view<T>::circular_view(T start, T sentinel) : m_begin(start), m_sentinel(sentinel), m_begin_from(start) {
}

template <circular_viewable T>
circular_view<T>::circular_view(T start, T sentinel, T start_from)
    : m_begin(start), m_sentinel(sentinel), m_begin_from(start_from) {
}

template <circular_viewable T>
circular_view<T>::circular_view(T start, T sentinel, iterator start_from)
    : circular_view(start, sentinel, start_from.m_current) {
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::begin() const {
    return iterator{m_begin, m_sentinel, m_begin_from};
}

template <circular_viewable T>
circular_view<T>::iterator circular_view<T>::end() const {
    return iterator{m_begin, m_sentinel, m_begin_from};
}

template <circular_viewable T>
circular_view<T> circular_view<T>::from(iterator it) const {
    return circular_view{m_begin, m_sentinel, it.m_current};
}

template <circular_viewable T>
circular_view<T> circular_view<T>::from(T it) const {
    return circular_view{m_begin, m_sentinel, it};
}

template <circular_viewable T>
circular_view<T> circular_view<T>::from(iterator::difference_type value) const {
    std::cout << "from(int) value: " << value << std::endl;
    iterator from_it = this->begin();
    from_it += value;
    return circular_view{m_begin, m_sentinel, from_it};
}

template <circular_viewable T>
size_t circular_view<T>::size() {
    return std::distance(m_begin, m_sentinel);
}

}  // namespace clover
