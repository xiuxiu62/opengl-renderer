#pragma once

#include "core/types.h"

// #include "miniaudio.h"

// #define MAX_SOUNDS 32

// struct AudioSystem {
//     ma_engine engine;
//     ma_sound sounds[MAX_SOUNDS];
//     u32 sound_count;
// };

bool audio_init();

void audio_deinit();

u32 audio_load(const char *path);

void audio_remove(u32 handle);

void audio_play(u32 handle);
