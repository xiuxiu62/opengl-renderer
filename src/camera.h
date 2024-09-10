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
};

namespace camera {

typedef Mat4 Uniform;

Camera *init(Viewport viewport, Vec2 postition = Vec2::ZERO(), f32 zoom = 1.0);
Uniform uniform(void);
void move(Vec2 offset);
void zoom(f32 amount);
void update_viewport(Viewport viewport);
Vec2 screen_to_world(const Vec2 &screen_coordinates);
Vec2 world_to_screen(const Vec2 &world_coordinates);
void update_view_matrix();
} // namespace camera
