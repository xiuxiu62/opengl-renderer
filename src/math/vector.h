#pragma once

#include "types.h"

struct Vec2 {
    f32 x, y;

    static inline constexpr Vec2 ZERO() {
        return {0, 0};
    }

    inline constexpr Vec2 operator+(const f32 s) const {
        return {x + s, y + s};
    }

    inline constexpr Vec2 operator+(const Vec2 &v) const {
        return {x + v.x, y + v.y};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
    }

    inline void operator+=(const Vec2 &v) {
        x += v.x;
        y += v.y;
    }

    inline constexpr Vec2 operator-(const f32 s) const {
        return {x - s, y - s};
    }

    inline constexpr Vec2 operator-(const Vec2 &v) const {
        return {x - v.x, y - v.y};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
    }

    inline void operator-=(const Vec2 &v) {
        x -= v.x;
        y -= v.y;
    }

    inline constexpr Vec2 operator*(const f32 s) const {
        return {x * s, y * s};
    }

    inline constexpr Vec2 operator*(const Vec2 &v) const {
        return {x * v.x, y * v.y};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
    }

    inline void operator*=(const Vec2 &v) {
        x *= v.x;
        y *= v.y;
    }

    inline constexpr Vec2 operator/(const f32 s) const {
        return {x - s, y - s};
    }

    inline constexpr Vec2 operator/(const Vec2 &v) const {
        return {x - v.x, y - v.y};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
    }

    inline void operator/=(const Vec2 &v) {
        x /= v.x;
        y /= v.y;
    }

    f32 dot(const Vec2 &v) const;
    f32 perp_dot(const Vec2 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec2 normalized() const;
};

struct Vec3 {
    f32 x, y, z;

    static inline constexpr Vec3 ZERO() {
        return {0, 0, 0};
    }

    inline constexpr Vec3 operator+(const f32 s) const {
        return {x + s, y + s, z + s};
    }

    inline constexpr Vec3 operator+(const Vec3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
        z += s;
    }

    inline void operator+=(const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    inline constexpr Vec3 operator-(const f32 s) const {
        return {x - s, y - s, z - s};
    }

    inline constexpr Vec3 operator-(const Vec3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
        z -= s;
    }

    inline void operator-=(const Vec3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline constexpr Vec3 operator*(const f32 s) const {
        return {x * s, y * s, z * s};
    }

    inline constexpr Vec3 operator*(const Vec3 &v) const {
        return {x * v.x, y * v.y, z * v.z};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
        z *= s;
    }

    inline void operator*=(const Vec3 &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    inline constexpr Vec3 operator/(const f32 s) const {
        return {x / s, y / s, z / s};
    }

    inline constexpr Vec3 operator/(const Vec3 &v) const {
        return {x / v.x, y / v.y, z / v.z};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
        z /= s;
    }

    inline void operator/=(const Vec3 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }

    f32 dot(const Vec3 &v) const;
    Vec3 cross(const Vec3 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec3 normalized() const;
};

struct Vec4 {
    f32 x, y, z, w;

    static inline constexpr Vec4 ZERO() {
        return {0, 0, 0, 0};
    }

    inline constexpr Vec4 operator+(const f32 s) const {
        return {x + s, y + s, z + s, w + s};
    }

    inline constexpr Vec4 operator+(const Vec4 &v) const {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
        z += s;
        w += s;
    }

    inline void operator+=(const Vec4 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.z;
    }

    inline constexpr Vec4 operator-(const f32 s) const {
        return {x - s, y - s, z - s, w - s};
    }

    inline constexpr Vec4 operator-(const Vec4 &v) const {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
        z -= s;
        w -= s;
    }

    inline void operator-=(const Vec4 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }

    inline constexpr Vec4 operator*(const f32 s) const {
        return {x * s, y * s, z * s, w * s};
    }

    inline constexpr Vec4 operator*(const Vec4 &v) const {
        return {x * v.x, y * v.y, z * v.z, w * v.w};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    }

    inline void operator*=(const Vec4 &v) {

        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
    }

    inline constexpr Vec4 operator/(const f32 s) const {
        return {x / s, y / s, z / s, w / s};
    }

    inline constexpr Vec4 operator/(const Vec4 &v) const {
        return {x / v.x, y / v.y, z / v.z, w / v.w};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
    }

    inline void operator/=(const Vec4 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
    }

    f32 dot(const Vec4 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec4 normalized() const;
};
