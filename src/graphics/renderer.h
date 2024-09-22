#pragma once

#include "core/types.h"

struct ParallaxLayer {
    f32 focal_distance; // 0 = focus, < 0 = moves faster than camera, > 0 = moves slower than camera
};

struct Renderer {
    ParallaxLayer parallax_layers;
};

struct Slice {
    void *ptr;
    usize len;
};

struct RenderPacket {
    Slice sprites;
    Slice tiles;
};

void renderer_init(void);
void renderer_deinit(void);
void renderer_begin(void);
void renderer_draw(RenderPacket packet);
void renderer_end(void);
