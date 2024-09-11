#include "core/types.h"
#include "math/constants.h"
#include "math/vector.h"

#include <cmath>
#include <stdio.h>

// Vec2
f32 Vec2::dot(const Vec2 &v) const {
    return x * v.x + y * v.y;
}

f32 Vec2::perp_dot(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

f32 Vec2::length_squared() const {
    return x * x + y * y;
}

f32 Vec2::length() const {
    return std::sqrt(length_squared());
}

Vec2 Vec2::normalized() const {
    f32 len = length();
    f32 inv_len = 1.0f / (len + EPSILON);
    f32 scale = len * inv_len;
    return {x * inv_len * scale, y * inv_len * scale};
}

// Vec3
f32 Vec3::dot(const Vec3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3 &v) const {
    return {
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x,
    };
}

f32 Vec3::length_squared() const {
    return x * x + y * y + z * z;
}

f32 Vec3::length() const {
    return std::sqrt(length_squared());
}

Vec3 Vec3::normalized() const {
    f32 len = length();
    f32 inv_len = 1.0f / (len + EPSILON);
    f32 scale = len * inv_len;
    return {x * inv_len * scale, y * inv_len * scale, z * inv_len * scale};
}

// Vec4
f32 Vec4::dot(const Vec4 &v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

f32 Vec4::length_squared() const {
    return x * x + y * y + z * z + w * w;
}

f32 Vec4::length() const {
    return std::sqrt(length_squared());
}

Vec4 Vec4::normalized() const {
    f32 len = length();
    f32 inv_len = 1.0f / (len + EPSILON);
    f32 scale = len * inv_len;
    return {x * inv_len * scale, y * inv_len * scale, z * inv_len, w * inv_len};
}
