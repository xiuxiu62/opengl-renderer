#pragma once

#include "math/vector.h"

#include <cmath>

struct Mat3 {
    Vec3 x, y, z;

    static inline constexpr Mat3 IDENTITY() {
        return {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
        };
    }

    inline constexpr Mat3 operator*(const Mat3 &m) const {
        return {x * m.x, y * m.y, z * m.z};
    }

    inline constexpr Mat3 operator*(const Vec3 &v) const {
        return {
            x * v,
            y * v,
            z * v,
        };
    }

    inline void operator*=(const Mat3 &m) {
        x *= m.x;
        y *= m.y;
        z *= m.z;
    }

    inline void operator*=(const Vec3 &v) {
        x *= v;
        y *= v;
        z *= v;
    }

    Mat3 translation(const Vec2 &v);
    Mat3 rotation(f32 angle);
    Mat3 scale(const Vec2 &v);

    // Mat3x3 transposed() const;
    // Mat3x3 inverse() const;
    // f32 determinant() const;
};

struct Mat4 {
    Vec4 x, y, z, w;

    static inline constexpr Mat4 IDENTITY() {
        return {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        };
    }

    inline constexpr Mat4 operator*(const Mat4 &m) const {
        return {x * m.x, y * m.y, z * m.z, w * m.w};
    }

    inline constexpr Mat4 operator*(const Vec4 &v) const {
        return {
            x * v,
            y * v,
            z * v,
            w * v,
        };
    }

    inline void operator*=(const Mat4 &m) {
        x *= m.x;
        y *= m.y;
        z *= m.z;
        w *= m.w;
    }

    inline void operator*=(const Vec4 &v) {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
    }

    static Mat4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
    static Mat4 translation(const Vec4 &v);
    static Mat4 rotation(f32 angle);
    static Mat4 scale(const Vec2 &v);
};
