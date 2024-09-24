#pragma once

#include "core/types.h"

template <typename T> struct Slice {
    T *ptr;
    usize len;

    T *begin();
    T *end();
    const T *begin() const;
    const T *end() const;

    T &operator[](usize i);
    const T &operator[](usize i) const;
};

template <typename T> T *Slice<T>::begin() {
    return ptr;
}

template <typename T> T *Slice<T>::end() {
    return ptr + sizeof(T) * len;
}

template <typename T> const T *Slice<T>::begin() const {
    return ptr;
}

template <typename T> const T *Slice<T>::end() const {
    return ptr + sizeof(T) * len;
}

template <typename T> T &Slice<T>::operator[](usize i) {
    return i < len ? ptr[i] : nullptr;
}

template <typename T> const T &Slice<T>::operator[](usize i) const {
    return i < len ? ptr[i] : nullptr;
}
