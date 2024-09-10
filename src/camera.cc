#include "camera.h"
#include "math/algorithm.h"
#include "math/vector.h"

const f32 ZOOM_MIN = 0.1;
const f32 ZOOM_MAX = 10;

static struct {
    Camera camera;
} local;

namespace camera {

Camera *init(Viewport viewport, Vec2 postition, f32 zoom) {
    local.camera = {
        .position = postition,
        .zoom = zoom,
        .viewport = viewport,
        .aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height),
    };
    update_view_matrix();
    return &local.camera;
} // namespace camera

Uniform uniform(void) {
    return local.camera.view_matrix;
}

void move(Vec2 offset) {
    local.camera.position += offset;
}

void zoom(f32 amount) {
    local.camera.zoom = math::clamp(local.camera.zoom + amount, ZOOM_MIN, ZOOM_MAX);
}

void update_viewport(Viewport viewport) {
    local.camera.viewport = viewport;
    local.camera.aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height);
}

Vec2 screen_to_world(const Vec2 &screen_coordinates) {
    Viewport viewport = local.camera.viewport;
    Vec2 position = local.camera.position;
    f32 zoom = local.camera.zoom;
    f32 aspect_ratio = local.camera.aspect_ratio;

    float x = (screen_coordinates.x / static_cast<f32>(viewport.width) - 0.5) * 2 * aspect_ratio / zoom + position.x;
    float y = (screen_coordinates.y / static_cast<f32>(viewport.height) - 0.5) * 2 / zoom + position.y;

    return {x, y};
}

Vec2 world_to_screen(const Vec2 &world_coordinates) {
    Viewport viewport = local.camera.viewport;
    Vec2 position = local.camera.position;
    f32 zoom = local.camera.zoom;
    f32 aspect_ratio = local.camera.aspect_ratio;

    float x = (world_coordinates.x - position.x) * zoom / (aspect_ratio * 2) + 0.5;
    float y = (world_coordinates.y - position.y) * zoom / 2 + 0.5;

    return {x * viewport.width, y * viewport.height};
}

inline void update_view_matrix() {
    Vec2 position = local.camera.position;
    f32 zoom = local.camera.zoom;
    f32 aspect_ratio = 1.0f / local.camera.aspect_ratio;
    f32 inv_zoom = 1.0f / local.camera.zoom;

    local.camera.view_matrix = {
        {inv_zoom / aspect_ratio, 0, 0, 0},
        {0, inv_zoom, 0, 0},
        {0, 0, 1, 0},
        {-position.x * inv_zoom, -position.y * inv_zoom, 0, 1},
    };
}
} // namespace camera
