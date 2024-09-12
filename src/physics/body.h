#pragma once

#include "math/rotor.h"
#include "math/vector.h"

enum RigidBodyKind {
    DYNAMIC, // Affected by external forces and contacts
    STATIC,  // Other bodies are still affected by it, but it is not afftected by other bodies
};

struct Velocity {
    Vec2 linear;
    Rot2 angular;
};

struct Dampening {
    Vec2 linear; // between 0.0 and 1.0
    f32 angular; // between 0.0 and 1.0
};

struct RigidBody {
    RigidBodyKind kind;
    Vec2 position, size;
    Velocity velocity;
    Dampening dampening;
    f32 mass,
        restitution, // bounciness
        moment_of_inertia;
};

constexpr RigidBody rigid_body_new(RigidBodyKind kind, Vec2 position, Vec2 size,
                                   Velocity velocity = {Vec2::ZERO(), Rot2::IDENTITY()},
                                   Dampening dampening = {{0.0f, 0.0f}, 0.0f}, f32 mass = 1.0, f32 restitution = 0.8) {
    return RigidBody{
        .kind = kind,
        .position = position,
        .size = size,
        .velocity =
            {
                .linear = Vec2::ZERO(),
                .angular = Rot2::IDENTITY(),
            },
        .dampening =
            {
                .linear = {0.0f, 0.0f},
                .angular = 0.0f,
            },
        .mass = mass,
        .moment_of_inertia = (1.0f / 12.0f) * mass * (size.x * size.x + size.y * size.y),
    };
}

// TODO: figure out Rot2 procs
// TODO: design how to store and manage transforms
void rigid_body_update(RigidBody &body, f32 delta_t) {
    if (body.kind == RigidBodyKind::STATIC) return;

    body.velocity.linear *= (Vec2{1.0f, 1.0f} - body.dampening.linear * delta_t);
    // body.velocity.angular *= (1.0f - body.dampening.angular * delta_t);

    body.position += body.velocity.linear * delta_t;
}

// TODO: figure out Rot2 procs
void apply_force(RigidBody &body, const Vec2 &force, const Vec2 &application_point) {
    if (body.kind == RigidBodyKind::STATIC) return;

    Vec2 acceleration = force / body.mass;
    body.velocity.linear += acceleration;

    Vec2 r = application_point - body.position;
    f32 torque = r.perp_dot(force);
    f32 angular_acceleration = torque / body.moment_of_inertia;
    // body.velocity.angular = body.velocity.angular.rotate_by(angular_acceleration);
}
