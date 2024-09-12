#pragma once

#include "math/vector.h"
#include "physics/body.h"

struct AABB {
    Vec2 min, max;
};

Vec2 aabb_size(const AABB &self);

constexpr AABB aabb_default() {
    return {
        .min = {0, 0},
        .max = {0, 0},
    };
}

inline AABB aabb_from(const RigidBody &body) {
    Vec2 half_size = body.size * 0.5f;
    return {
        .min = body.position - half_size,
        .max = body.position + half_size,
    };
}

inline AABB aabb_from(const Vec2 &center, const Vec2 &size) {
    Vec2 half_size = size * 0.5f;
    return {
        .min = center - half_size,
        .max = center + half_size,
    };
}

// Update the AABB based on a new center position
inline void aabb_update(AABB &self, const Vec2 &new_center) {
    Vec2 half_size = aabb_size(self) * 0.5f;
    self.min = new_center - half_size;
    self.max = new_center + half_size;
}

inline bool aabb_overlaps(const AABB &self, const AABB &other) {
    return (self.min.x <= other.max.x && self.max.x >= other.min.x) &&
           (self.min.y <= other.max.y && self.max.y >= other.min.y);
}

inline Vec2 aabb_center(const AABB &self) {
    return (self.min + self.max) * 0.5f;
}

inline Vec2 aabb_size(const AABB &self) {
    return self.max - self.min;
}

struct CollisionInfo {
    Vec2 normal; // points from A to B
    f32 depth;   // penetration depth
};

// Function to find collision information between two AABBs
CollisionInfo find_collision_info(const AABB &a, const AABB &b) {
    CollisionInfo info;
    Vec2 center_diff = aabb_center(b) - aabb_center(a);
    Vec2 size_a = aabb_size(a), size_b = aabb_size(b);

    // Find the overlap on each axis
    f32 overlap_x = size_a.x * 0.5f + size_b.x * 0.5f - std::abs(center_diff.x),
        overlap_y = size_a.y * 0.5f + size_b.y * 0.5f - std::abs(center_diff.y);

    // Choose the axis with the smallest overlap
    if (overlap_x < overlap_y) {
        info.normal = {center_diff.x > 0.0f ? 1.0f : -1.0f, 0.0f};
        info.depth = overlap_x;
    } else {
        info.normal = {0.0f, center_diff.y > 0.0f ? 1.0f : -1.0f};
        info.depth = overlap_y;
    }

    return info;
}

bool check_collision(const RigidBody &a, const RigidBody &b) {
    return aabb_overlaps(aabb_from(a), aabb_from(b));
}

void resolve_collision(RigidBody &a, RigidBody &b) {
    AABB aabb_a = aabb_from(a), aabb_b = aabb_from(b);
    if (!aabb_overlaps(aabb_a, aabb_b)) return;
    CollisionInfo info = find_collision_info(aabb_a, aabb_b);

    Vec2 relative_velocity = b.velocity.linear - a.velocity.linear;
    f32 velocity_along_normal = relative_velocity.dot(info.normal);
    if (velocity_along_normal > 0) return;

    f32 inv_mass_a = a.kind == RigidBodyKind::DYNAMIC ? 1.0f / a.mass : 0.0f,
        inv_mass_b = b.kind == RigidBodyKind::DYNAMIC ? 1.0f / b.mass : 0.0f;

    f32 e = std::fmin(a.restitution, b.restitution) * 0.5f + std::fmax(a.restitution, b.restitution) * 0.5f,
        j = (-1 + e) * velocity_along_normal / (inv_mass_a + inv_mass_b);

    Vec2 impulse = info.normal * j;
    if (a.kind == RigidBodyKind::DYNAMIC) a.velocity.linear -= impulse / a.mass;
    if (b.kind == RigidBodyKind::DYNAMIC) b.velocity.linear += impulse / b.mass;

    // Positional correction to prevent sinking
    static f32 percent = 0.2; // in range 20% to 80%
    static f32 slop = 0.01;   // in range 1% to 10%
    Vec2 correction = info.normal * (std::fmax(info.depth - slop, 0.0f) / (inv_mass_a + inv_mass_b) * percent);
    if (a.kind == RigidBodyKind::DYNAMIC) a.position -= correction / a.mass;
    if (b.kind == RigidBodyKind::DYNAMIC) b.position += correction / b.mass;
}

void resolve_collisions(RigidBody *bodies, usize body_count) {
    for (usize i = 0; i < body_count; i++)
        for (usize j = 0; j < body_count; j++)
            if (i != j) resolve_collision(bodies[i], bodies[j]);
}

void physics_update(RigidBody *bodies, usize body_count, f32 delta_t) {
    for (usize i = 0; i < body_count; i++)
        rigid_body_update(bodies[i], delta_t);
    resolve_collisions(bodies, body_count);
}
