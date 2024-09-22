#include "camera.h"
#include "math/algorithm.h"
#include "math/matrix.h"
#include "math/vector.h"

const f32 ZOOM_MIN = 5.0f;
const f32 ZOOM_MAX = 15.0f;

Camera camera_create(Viewport viewport, Vec2 postition, f32 zoom) {
    Camera camera{
        .position = postition,
        .zoom = zoom,
        .viewport = viewport,
        .aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height),
    };
    camera_update_matrix(camera);
    return camera;
}

void camera_move(Camera &camera, Vec2 offset) {
    camera.position += offset;
}

void camera_zoom(Camera &camera, f32 factor) {
    camera.zoom = math::clamp(camera.zoom * factor, ZOOM_MIN, ZOOM_MAX);
}

void camera_update_viewport(Camera &camera, Viewport viewport) {
    camera.viewport = viewport;
    camera.aspect_ratio = static_cast<f32>(viewport.width) / static_cast<f32>(viewport.height);
}

Vec2 camera_screen_to_world(Camera &camera, const Vec2 &screen_coordinates) {
    Vec2 normalized = {
        screen_coordinates.x / camera.viewport.width,
        screen_coordinates.y / camera.viewport.height,
    };
    return {
        camera.position.x + (normalized.x - 0.5f) * camera.viewport.width / camera.zoom,
        camera.position.y + (0.5f - normalized.y) * camera.viewport.height / camera.zoom,
    };
}

Vec2 camera_world_to_screen(Camera &camera, const Vec2 &world_coordinates) {
    Vec2 relative = {
        (world_coordinates.x - camera.position.x) * camera.zoom,
        (camera.position.y - world_coordinates.y) * camera.zoom,
    };
    return {
        (relative.x + camera.viewport.width * 0.5f),
        (relative.y + camera.viewport.height * 0.5f),
    };
}

void camera_update_matrix(Camera &camera) {
    Mat4 scale = Mat4::scale({camera.zoom, camera.zoom, 1.0f});
    Mat4 translation = Mat4::translation({-camera.position.x, -camera.position.y, 0.0f});

    // Orthographic projection
    f32 width = camera.viewport.width / camera.zoom;
    f32 height = camera.viewport.height / camera.zoom;
    f32 right = width * 0.5f;
    f32 left = -right;
    f32 top = height * 0.5f;
    f32 bottom = -top;
    f32 far = 1.0f;
    f32 near = -1.0f;
    Mat4 ortho = Mat4::ortho(left, right, bottom, top, near, far);

    // Combine transformations
    camera.matrix = ortho * scale * translation;
}
