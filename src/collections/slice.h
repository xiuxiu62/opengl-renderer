#pragma once

#include "core/types.h"

template <typename T> struct Slice {
    T *ptr;
    usize len;

    Slice<T> create(T *ptr, usize len) {
        return {ptr, len};
    }

    T *begin() {
        return ptr;
    }

    T *end() {
        return ptr + sizeof(T) * len;
    }

    const T *begin() const {
        return ptr;
    }

    const T *end() const {
        return ptr + sizeof(T) * len;
    }
};
