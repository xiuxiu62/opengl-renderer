#pragma once

#include "core/time/timer.h"
#include "math/transform.h"
#include "math/vector.h"
#include "resources/image.h"
#include "sprite/sprite.h"

struct SpriteSheet {
    GenHandle image_handle;
    u16 frame_width, frame_height;
    u16 frame_count;
    u8 rows, columns;
};

struct SpriteAnimation {
    // const SpriteSheet &sheet;
    Timer frame_timer;
    u8 sheet_row;
    u16 current_frame;
    bool looping;
};

struct AnimatedSprite {
    Sprite sprite;
    SpriteSheet sprite_sheet;
    SpriteAnimation *animations;
    usize animation_count;

    AnimatedSprite create(f32 width, f32 height, Transform transform, Texture texture, u16 frame_width,
                          u16 frame_height, u8 sheet_rows, u8 sheet_columns, u8 *animation_strides);
};

SpriteSheet sprite_sheet_create(GenHandle image_handle, u16 frame_width, u16 frame_height, u16 frame_count);

SpriteAnimation sprite_animation_create(const SpriteSheet &sheet, f64 frame_duration, bool looping = true);

void animated_sprite_calculate_vertices(AnimatedSprite &sprite, Sprite::Vertex vertices[4]);

void update_sprite_animation(SpriteAnimation &animation, f32 delta_t);

Vec4 get_current_frame_uv(const SpriteAnimation &animation);

// new
struct Temp_SpriteSheet {
    GenHandle image_handle;
    u32 total_width, total_height;
};

struct Temp_SpriteAnimation {
    u16 start_frame, end_frame;
    u8 row, columns;
    f64 frame_duration;
    bool looping;
};

struct Temp_AnimatedSprite {
    Sprite sprite;
    Temp_SpriteSheet sprite_sheet;
    SpriteAnimation *animations;
    u8 animation_count, current_animation;
    Timer frame_timer;
    u16 current_frame;

    static AnimatedSprite create(f32 width, f32 height, Transform transform, Texture texture, u16 total_sheet_width,
                                 u16 total_sheet_height, u8 rows, u8 columns, SpriteAnimation *animations,
                                 u8 animation_count);
};
