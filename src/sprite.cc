#include "math/vector.h"
#include "sprite.h"

void sprite_calculate_vertices(const Sprite *sprite, Sprite::Vertex out_vertices[VERTEX_COUNT]) {
    f32 half_width = sprite->width / 2;
    f32 half_height = sprite->height / 2;

    Vec2 positions[4] = {
        {-half_width, -half_height}, // Bottom-left
        {half_width, -half_height},  // Bottom-right
        {half_width, half_height},   // Top-right
        {-half_width, half_height}   // Top-left
    };

    constexpr Vec3 COLORS[4]{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
    };

    for (usize i = 0; i < VERTEX_COUNT; i++) {
        out_vertices[i].position = sprite->transform.rotation.rotate(positions[i]) + sprite->transform.position;
        out_vertices[i].uv = (positions[i] / Vec2{sprite->width, sprite->height}) + 0.5f;
        out_vertices[i].normal = {0, 0};
        out_vertices[i].color = COLORS[i];
    }
}
