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
        return {
            {
                x.x * m.x.x + x.y * m.y.x + x.z * m.z.x + x.w * m.w.x,
                x.x * m.x.y + x.y * m.y.y + x.z * m.z.y + x.w * m.w.y,
                x.x * m.x.z + x.y * m.y.z + x.z * m.z.z + x.w * m.w.z,
                x.x * m.x.w + x.y * m.y.w + x.z * m.z.w + x.w * m.w.w,
            },
            {
                y.x * m.x.x + y.y * m.y.x + y.z * m.z.x + y.w * m.w.x,
                y.x * m.x.y + y.y * m.y.y + y.z * m.z.y + y.w * m.w.y,
                y.x * m.x.z + y.y * m.y.z + y.z * m.z.z + y.w * m.w.z,
                y.x * m.x.w + y.y * m.y.w + y.z * m.z.w + y.w * m.w.w,
            },
            {
                z.x * m.x.x + z.y * m.y.x + z.z * m.z.x + z.w * m.w.x,
                z.x * m.x.y + z.y * m.y.y + z.z * m.z.y + z.w * m.w.y,
                z.x * m.x.z + z.y * m.y.z + z.z * m.z.z + z.w * m.w.z,
                z.x * m.x.w + z.y * m.y.w + z.z * m.z.w + z.w * m.w.w,
            },
            {
                w.x * m.x.x + w.y * m.y.x + w.z * m.z.x + w.w * m.w.x,
                w.x * m.x.y + w.y * m.y.y + w.z * m.z.y + w.w * m.w.y,
                w.x * m.x.z + w.y * m.y.z + w.z * m.z.z + w.w * m.w.z,
                w.x * m.x.w + w.y * m.y.w + w.z * m.z.w + w.w * m.w.w,
            },
        };
    }

    inline constexpr Mat4 operator*(const Vec4 &v) const {
        return {
            x.x * v.x + x.y * v.y + x.z * v.z + x.w * v.w,
            y.x * v.x + y.y * v.y + y.z * v.z + y.w * v.w,
            z.x * v.x + z.y * v.y + z.z * v.z + z.w * v.w,
            w.x * v.x + w.y * v.y + w.z * v.z + w.w * v.w,
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

    static Mat4 ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);
    static Mat4 translation(const Vec4 &v);
    static Mat4 rotation(f32 angle);
    static Mat4 scale(const Vec2 &v);
};
