#pragma once

#include "core/types.h"
#include "math/vector.h"

#include <cmath>

struct Rot2 {
    f32 scalar;
    f32 bivector;

    Rot2() : scalar(1), bivector(0) {
    }

    Rot2(f32 scalar, f32 bivector) : scalar(scalar), bivector(bivector) {
    }

    // Constructor from rotation angle
    explicit Rot2(float angle) {
        f32 half_angle = angle * 0.5f;
        *this = Rot2(std::cos(half_angle), std::sin(half_angle));
    }

    Vec2 rotate(const Vec2 &v) const {
        f32 x = v.x * (scalar * scalar - bivector * bivector) + v.y * (-2 * scalar * bivector);
        f32 y = v.x * (2 * scalar * bivector) + v.y * (scalar * scalar - bivector * bivector);
        return {x, y};
    }

    Rot2 operator*(const Rot2 &other) const {
        return Rot2(scalar * other.scalar - bivector * other.bivector,
                    scalar * other.bivector + bivector * other.scalar);
    }
};

// constexpr Mat3x3 Mat3x3::IDENTITY{
//     {1, 0, 0},
//     {0, 1, 0},
//     {0, 0, 1},
// };
