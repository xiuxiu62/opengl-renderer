#pragma once

#include "math/transform.h"
#include "math/vector.h"
#include "types.h"

struct Sprite {
    f32 width, height;
    Transform transform;

    struct Vertex {
        Vec2 position;
        Vec2 uv;
        Vec2 normal;
        Vec3 color;
    };

    typedef u32 Index;
};

constexpr usize VERTEX_COUNT = 4;
constexpr usize INDEX_COUNT = 6;
constexpr u32 INDICES[INDEX_COUNT] = {0, 1, 2, 2, 3, 0};

constexpr usize VERTEX_STORAGE_SIZE = sizeof(Sprite::Vertex) * VERTEX_COUNT;
constexpr usize INDEX_STORAGE_SIZE = sizeof(Sprite::Index) * INDEX_COUNT;

constexpr u32 ATTR_COUNT = 4;

void sprite_calculate_vertices(const Sprite *sprite, Sprite::Vertex out_vertices[VERTEX_COUNT]);
