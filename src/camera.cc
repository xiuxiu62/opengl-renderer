#include "camera.h"
#include "math/algorithm.h"
#include "math/vector.h"

const f32 ZOOM_MIN = 0.1;
const f32 ZOOM_MAX = 10;

// namespace camera {

// Camera *init(Viewport viewport, Vec2 postition, f32 zoom) {
//     camera = {
//         .position = postition,
//         .zoom = zoom,
//         .viewport = viewport,
//         .aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height),
//     };
//     update_view_matrix();
//     return &camera;
// } // namespace camera

// Camera::Uniform uniform(void) {
//     return camera->view_matrix;
// }

// void move(Vec2 offset) {
//     camera->position += offset;
// }

// void zoom(f32 amount) {
//     camera->zoom = math::clamp(camera->zoom + amount, ZOOM_MIN, ZOOM_MAX);
// }

// void update_viewport(Viewport viewport) {
//     camera->viewport = viewport;
//     camera->aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height);
// }

// Vec2 screen_to_world(const Vec2 &screen_coordinates) {
//     Viewport viewport = camera->viewport;
//     Vec2 position = camera->position;
//     f32 zoom = camera->zoom;
//     f32 aspect_ratio = camera->aspect_ratio;

//     float x = (screen_coordinates.x / static_cast<f32>(viewport.width) - 0.5) * 2 * aspect_ratio / zoom + position.x;
//     float y = (screen_coordinates.y / static_cast<f32>(viewport.height) - 0.5) * 2 / zoom + position.y;

//     return {x, y};
// }

// Vec2 world_to_screen(const Vec2 &world_coordinates) {
//     Viewport viewport = camera->viewport;
//     Vec2 position = camera->position;
//     f32 zoom = camera->zoom;
//     f32 aspect_ratio = camera->aspect_ratio;

//     float x = (world_coordinates.x - position.x) * zoom / (aspect_ratio * 2) + 0.5;
//     float y = (world_coordinates.y - position.y) * zoom / 2 + 0.5;

//     return {x * viewport.width, y * viewport.height};
// }

// inline void update_view_matrix() {
//     Vec2 position = camera->position;
//     f32 zoom = camera->zoom;
//     f32 aspect_ratio = 1.0f / camera->aspect_ratio;
//     f32 inv_zoom = 1.0f / camera->zoom;

//     camera->view_matrix = {
//         {inv_zoom / aspect_ratio, 0, 0, 0},
//         {0, inv_zoom, 0, 0},
//         {0, 0, 1, 0},
//         {-position.x * inv_zoom, -position.y * inv_zoom, 0, 1},
//     };
// }
// } // namespace camera

void camera_update_view_matrix(Camera *camera);

Camera camera_create(Viewport viewport, Vec2 postition, f32 zoom) {
    Camera camera{
        .position = postition,
        .zoom = zoom,
        .viewport = viewport,
        .aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height),
    };
    camera_update_view_matrix(&camera);
    return camera;
}

void camera_move(Camera *camera, Vec2 offset) {
    camera->position += offset;
}

void camera_zoom(Camera *camera, f32 amount) {
    camera->zoom = math::clamp(camera->zoom + amount, ZOOM_MIN, ZOOM_MAX);
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

void camera_update_view_matrix(Camera *camera) {
    Vec2 position = camera->position;
    f32 zoom = camera->zoom;
    f32 aspect_ratio = 1.0f / camera->aspect_ratio;
    f32 inv_zoom = 1.0f / camera->zoom;

    camera->view_matrix = {
        {inv_zoom / aspect_ratio, 0, 0, 0},
        {0, inv_zoom, 0, 0},
        {0, 0, 1, 0},
        {-position.x * inv_zoom, -position.y * inv_zoom, 0, 1},
    };
}
