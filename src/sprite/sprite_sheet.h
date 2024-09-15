#pragma once

#include "core/time/timer.h"
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
    const SpriteSheet &sheet;
    Timer frame_timer;
    u16 current_frame;
    bool looping;
};

struct AnimatedSprite {
    Sprite sprite;
    SpriteAnimation animation;
};

SpriteSheet sprite_sheet_create(GenHandle image_handle, u16 frame_width, u16 frame_height, u16 frame_count);

SpriteAnimation sprite_animation_create(const SpriteSheet &sheet, f64 frame_duration, bool looping = true);

void animated_sprite_calculate_vertices(AnimatedSprite &sprite, Sprite::Vertex vertices[4]);

void update_sprite_animation(SpriteAnimation &animation, f32 delta_t);

Vec4 get_current_frame_uv(const SpriteAnimation &animation);
