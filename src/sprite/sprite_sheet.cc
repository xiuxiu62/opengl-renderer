#include "sprite/sprite_sheet.h"

// AnimatedSprite AnimatedSprite::create(f32 width, f32 height, Transform transform, Texture texture) {
//     Sprite sprite{
//         .width = width,
//         .height = height,
//         .transform = transform,
//         .texture = texture,
//     };

//     // calculate vertices
//     f32 half_width = sprite.width / 2;
//     f32 half_height = sprite.height / 2;

//     Vec2 positions[4] = {
//         {-half_width, -half_height}, // bottom-left
//         {half_width, -half_height},  // bottom-right
//         {half_width, half_height},   // top-right
//         {-half_width, half_height}   // top-left
//     };

//     static Vec3 normal = {0, 0, 1};
//     static Vec3 debug_colors[4]{
//         {1, 0, 0},
//         {0, 1, 0},
//         {0, 0, 1},
//         {1, 0, 1},
//     };

//     for (usize i = 0; i < Sprite::VERTEX_COUNT; i++) {
//         sprite.vertices[i].position = positions[i];
//         sprite.vertices[i].uv = (positions[i] / Vec2{sprite.width, sprite.height}) + 0.5f;
//         sprite.vertices[i].normal = normal;
//         sprite.vertices[i].color = debug_colors[i];
//     }

//     return sprite;
// }

// SpriteSheet sprite_sheet_create(GenHandle image_handle, u16 frame_width, u16 frame_height, u16 frame_count) {
//     Image *image = image_get(image_handle);
//     u8 columns = image->width / frame_width, //
//         rows = image->height / frame_height;

//     return {
//         .image_handle = image_handle,
//         .frame_width = frame_width,
//         .frame_height = frame_height,
//         .frame_count = frame_count,
//         .rows = rows,
//         .columns = columns,
//     };
// }

// SpriteAnimation sprite_animation_create(const SpriteSheet &sheet, f64 frame_duration, bool looping) {
//     return {
//         .sheet = sheet,
//         .frame_timer = timer_create(frame_duration, looping),
//         .current_frame = 0,
//         .looping = looping,
//     };
// }

// void update_sprite_animation(SpriteAnimation &animation, f32 delta_t) {
//     timer_update(&animation.frame_timer, delta_t);

//     if (!timer_is_finished(&animation.frame_timer)) {
//         animation.current_frame++;

//         // reset frame head if it has reached the end
//         if (animation.current_frame >= animation.sheet.frame_count) {
//             if (animation.looping) {
//                 animation.current_frame = 0;
//             } else {
//                 animation.current_frame = animation.sheet.frame_count - 1;
//             }
//         }

//         timer_reset(&animation.frame_timer);
//     }
// }

// // TODO: ensure this works as intended
// Vec4 get_current_frame_uv(const SpriteAnimation &animation) {
//     const SpriteSheet &sheet = animation.sheet;
//     u32 frame = animation.current_frame;

//     f32 u_width = static_cast<f32>(sheet.frame_width) / static_cast<f32>(sheet.columns);
//     f32 v_height = static_cast<f32>(sheet.frame_height) / static_cast<f32>(sheet.rows);

//     f32 u = (frame % sheet.columns) * u_width;
//     f32 v = (frame / sheet.columns) * v_height;

//     return {u, v, u + u_width, v + v_height};
// }
