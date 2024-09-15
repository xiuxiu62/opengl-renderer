#include "math/vector.h"
#include "sprite/sprite.h"

constexpr usize Sprite::VERTEX_COUNT;
constexpr usize Sprite::INDEX_COUNT;
constexpr u32 Sprite::INDICES[INDEX_COUNT];

constexpr usize Sprite::VERTEX_STORAGE_SIZE;
constexpr usize Sprite::INDEX_STORAGE_SIZE;

void populate_vertices(Sprite &sprite);

Sprite Sprite::create(f32 width, f32 height, Transform transform, Texture texture) {
    Sprite self{
        .width = width,
        .height = height,
        .transform = transform,
        .texture = texture,
    };

    populate_vertices(self);
    return self;
}

void populate_vertices(Sprite &sprite) {
    f32 half_width = sprite.width / 2;
    f32 half_height = sprite.height / 2;

    Vec2 positions[4] = {
        {-half_width, -half_height}, // Bottom-left
        {half_width, -half_height},  // Bottom-right
        {half_width, half_height},   // Top-right
        {-half_width, half_height}   // Top-left
    };

    constexpr Vec3 NORMAL = {0, 0, 1};
    constexpr Vec3 COLORS[4]{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
    };

    for (usize i = 0; i < Sprite::VERTEX_COUNT; i++) {
        sprite.vertices[i].position = positions[i];
        sprite.vertices[i].uv = (positions[i] / Vec2{sprite.width, sprite.height}) + 0.5f;
        sprite.vertices[i].normal = NORMAL;
        sprite.vertices[i].color = COLORS[i];
    }
}
