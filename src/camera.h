#pragma once

#include "core/types.h"
#include "math/matrix.h"
#include "math/vector.h"

struct Viewport {
    u32 width, height;
};

struct Camera {
    Vec2 position;
    // f32 zoom;
    f32 ortho_size; // large value = zoomed out, small value = zoomed in
    Viewport viewport;
    Mat4 matrix;

    typedef Mat4 Uniform;
};

Camera camera_create(Viewport viewport, Vec2 postition = Vec2::ZERO(), f32 ortho_size = 10.0f);

void camera_move(Camera &camera, Vec2 offset);
// void camera_update_position(Camera &camera, Vec2 position);

void camera_zoom(Camera &camera, f32 amount);

void camera_update_viewport(Camera &camera, Viewport viewport);

Vec2 camera_screen_to_world(Camera &camera, const Vec2 &screen_coordinates);

Vec2 camera_world_to_screen(Camera &camera, const Vec2 &world_coordinates);

void camera_update_matrix(Camera &camera);
