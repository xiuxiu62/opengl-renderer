#pragma once

#include "camera.h"
#include "graphics/light/point.h"
#include "graphics/program.h"
#include "sprite/sprite.h"
#include "sprite/sprite_sheet.h"

struct SpriteRenderer {
    Program program;
    u32 vertex_array, vertex_buffer, element_buffer;
};

void sprite_renderer_init(void);
void sprite_renderer_deinit(void);
void sprite_renderer_begin(Camera &camera, PointLight &light);
void sprite_renderer_draw(const Sprite &sprite);
void sprite_renderer_draw(AnimatedSprite &sprite);
void sprite_renderer_end(void);
