#pragma once

#include "camera.h"
#include "graphics/program.h"
#include "sprite.h"

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
