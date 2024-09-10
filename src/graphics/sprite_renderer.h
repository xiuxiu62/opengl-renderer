#pragma once

#include "camera.h"
#include "graphics/program.h"
#include "sprite.h"

// clang-format off
static constexpr Sprite::Vertex vertices[]{
    { // top right
        .position = {0.5, 0.5},
        .uv = {1, 1},
        .normal = {},
        .color = {},
    },
    { // bottom right
        .position = {0.5, -0.5},
        .uv = {1, -1},
        .normal = {},
        .color = {},
    },
    { // bottom left
        .position = {-0.5, -0.5},
        .uv = {-1, -1},
        .normal = {},
        .color = {},
    },
    { // top left
        .position = {-0.5f, 0.5f},
        .uv = {-1, 1},
        .normal = {},
        .color = {},
    },
}; // clang-format on

// clang-format off
static constexpr Sprite::Index indices[]{
    0, 1, 3,
    1, 2, 3,
}; // clang-format on

struct SpriteRenderer {
    Program program;
    u32 vertex_array, vertex_buffer, element_buffer;
    Camera *camera;
};

SpriteRenderer sprite_renderer_create(Camera *camera);

void sprite_renderer_destroy(SpriteRenderer *renderer);

void sprite_renderer_begin(SpriteRenderer *renderer);

void sprite_renderer_draw(SpriteRenderer *renderer);

void sprite_renderer_end(SpriteRenderer *renderer);
