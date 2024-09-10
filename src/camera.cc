#include "camera.h"
#include "math/algorithm.h"
#include "math/matrix.h"
#include "math/vector.h"
#include <iostream>

const f32 ZOOM_MIN = 0.1;
const f32 ZOOM_MAX = 10;

void camera_update_matrices(Camera *camera);

Camera camera_create(Viewport viewport, Vec2 postition, f32 zoom) {
    Camera camera{
        .position = postition,
        .zoom = zoom,
        .viewport = viewport,
        .aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height),
    };
    camera_update_matrices(&camera);
    return camera;
}

void camera_move(Camera *camera, Vec2 offset) {
    camera->position += offset;
}

void camera_zoom(Camera *camera, f32 factor) {
    // camera->zoom = math::clamp(camera->zoom * factor, ZOOM_MIN, ZOOM_MAX);
    camera->zoom *= factor;
}

void camera_update_viewport(Camera *camera, Viewport viewport) {
    camera->viewport = viewport;
    camera->aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height);
}

Vec2 camera_screen_to_world(Camera *camera, const Vec2 &screen_coordinates) {
    Viewport viewport = camera->viewport;
    Vec2 position = camera->position;
    f32 zoom = camera->zoom;
    f32 aspect_ratio = camera->aspect_ratio;

    float x = (screen_coordinates.x / static_cast<f32>(viewport.width) - 0.5) * 2 * aspect_ratio / zoom + position.x;
    float y = (screen_coordinates.y / static_cast<f32>(viewport.height) - 0.5) * 2 / zoom + position.y;

    return {x, y};
}

Vec2 camera_world_to_screen(Camera *camera, const Vec2 &world_coordinates) {
    Viewport viewport = camera->viewport;
    Vec2 position = camera->position;
    f32 zoom = camera->zoom;
    f32 aspect_ratio = camera->aspect_ratio;

    float x = (world_coordinates.x - position.x) * zoom / (aspect_ratio * 2) + 0.5;
    float y = (world_coordinates.y - position.y) * zoom / 2 + 0.5;

    return {x * viewport.width, y * viewport.height};
}

void camera_update_matrices(Camera *camera) {
    f32 half_width = camera->viewport.width / (2.0 * camera->zoom);
    f32 half_height = camera->viewport.height / (2.0 * camera->zoom);

    camera->projection_matrix = Mat4::ortho(-half_width, half_width, -half_height, half_height, -1, 1);
    camera->view_matrix = Mat4::translation({-camera->position.x, -camera->position.y, 0, 0});
    camera->combined_matrix = camera->projection_matrix * camera->view_matrix;
}
