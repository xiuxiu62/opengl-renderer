#pragma once

#include "core/types.h"
#include "math/transform.h"
#include "math/vector.h"

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

    static constexpr usize VERTEX_COUNT = 4;
    static constexpr usize INDEX_COUNT = 6;
    static constexpr u32 INDICES[INDEX_COUNT] = {0, 1, 2, 2, 3, 0};

    static constexpr usize VERTEX_STORAGE_SIZE = sizeof(Sprite::Vertex) * VERTEX_COUNT;
    static constexpr usize INDEX_STORAGE_SIZE = sizeof(Sprite::Index) * INDEX_COUNT;

    static constexpr u32 ATTR_COUNT = 4;
};

void sprite_calculate_vertices(const Sprite *sprite, Sprite::Vertex out_vertices[Sprite::VERTEX_COUNT]);
