#pragma once

#include "graphics/program.h"

// clang-format off
static constexpr f32 vertices[]{
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
}; // clang-format on

// clang-format off
static constexpr u32 indices[]{
    0, 1, 3,
    1, 2, 3,
}; // clang-format on

struct SpriteRenderer {
    Program program;
    u32 vertex_array, vertex_buffer, element_buffer;
};

SpriteRenderer sprite_renderer_create();
void sprite_renderer_destroy(SpriteRenderer *renderer);
void sprite_renderer_draw(SpriteRenderer *renderer);
