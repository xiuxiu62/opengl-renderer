#pragma once

#include "core/types.h"
#include "graphics/texture.h"
#include "math/transform.h"
#include "math/vector.h"

struct Sprite {
    struct Vertex {
        Vec2 position;
        Vec2 uv;
        Vec3 normal;
        Vec3 color;
    };

    f32 width, height;
    Transform transform;
    Texture texture;
    Vertex vertices[4];

    typedef u32 Index;

    static Sprite create(f32 width, f32 height, Transform transform, Texture texture);

    static constexpr usize VERTEX_COUNT = 4;
    static constexpr usize INDEX_COUNT = 6;
    static constexpr Index INDICES[INDEX_COUNT] = {
        0, 1, 2, // top left
        2, 3, 0, // bottom right
    };

    static constexpr usize VERTEX_STORAGE_SIZE = sizeof(Sprite::Vertex) * VERTEX_COUNT;
    static constexpr usize INDEX_STORAGE_SIZE = sizeof(Sprite::Index) * INDEX_COUNT;
};

const Sprite::Vertex *sprite_vertices(Sprite &sprite);
