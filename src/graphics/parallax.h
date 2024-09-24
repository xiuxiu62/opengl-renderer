#pragma once

#include "camera.h"
#include "graphics/texture.h"
#include "math/vector.h"

#define NUM_LAYERS 5

struct Layer {
    f32 parallax_factor;
    Vec2 position;
};

struct ParallaxManager {
    Layer layers[NUM_LAYERS];
};

static ParallaxManager manager;

static void parallax_init(void) {
    manager.layers[0] = {1.2f, Vec2::ZERO()};
    manager.layers[1] = {1.0f, Vec2::ZERO()};
    manager.layers[2] = {0.8f, Vec2::ZERO()};
    manager.layers[3] = {0.4f, Vec2::ZERO()};
    manager.layers[4] = {0.0f, Vec2::ZERO()};
}

static void parallax_update(Camera &camera, Vec2 camera_offset) {
    for (usize i = 0; i < NUM_LAYERS; i++)
        manager.layers[i].position += camera_offset * manager.layers[i].parallax_factor;
}
