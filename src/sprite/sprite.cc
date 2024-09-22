#include "math/vector.h"
#include "sprite/sprite.h"

constexpr usize Sprite::VERTEX_COUNT;
constexpr usize Sprite::INDEX_COUNT;
constexpr u32 Sprite::INDICES[INDEX_COUNT];

constexpr usize Sprite::VERTEX_STORAGE_SIZE;
constexpr usize Sprite::INDEX_STORAGE_SIZE;

Sprite Sprite::create(f32 width, f32 height, Transform transform, Texture texture) {
    Sprite sprite{
        .width = width,
        .height = height,
        .transform = transform,
        .texture = texture,
    };

    // calculate vertices
    f32 half_width = sprite.width / 2;
    f32 half_height = sprite.height / 2;

    Vec2 positions[4] = {
        {-half_width, half_height},  // top-left
        {half_width, half_height},   // top-right
        {half_width, -half_height},  // bottom-right
        {-half_width, -half_height}, // bottom-left
    };

    static Vec3 normal = {0, 0, 1};
    static Vec3 debug_colors[4]{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
    };

    for (usize i = 0; i < Sprite::VERTEX_COUNT; i++) {
        sprite.vertices[i].position = positions[i];
        sprite.vertices[i].uv =
            Vec2{
                (positions[i].x / sprite.width) + 0.5f,
                (positions[i].y / sprite.height) + 0.5f,
            },
        sprite.vertices[i].normal = normal;
        sprite.vertices[i].color = debug_colors[i];
    }

    return sprite;
}
