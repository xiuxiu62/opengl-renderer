#pragma once

#include "math/matrix.h"
#include "math/vector.h"
#include "types.h"

struct Viewport {
    u32 width, height;
};

struct Camera {
    Vec2 position;
    f32 zoom;
    Viewport viewport;
    f32 aspect_ratio;
    Mat4 view_matrix;
    Mat4 projection_matrix;
    Mat4 combined_matrix;

    typedef Mat4 Uniform;
};

Camera camera_create(Viewport viewport, Vec2 postition = Vec2::ZERO(), f32 zoom = 1);

void camera_move(Camera *camera, Vec2 offset);

void camera_zoom(Camera *camera, f32 amount);

void camera_update_viewport(Camera *camera, Viewport viewport);

Vec2 camera_screen_to_world(Camera *camera, const Vec2 &screen_coordinates);

Vec2 camera_world_to_screen(Camera *camera, const Vec2 &world_coordinates);

void camera_update_matrices(Camera *camera);
