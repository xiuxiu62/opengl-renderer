#define MINIAUDIO_IMPLEMENTATION

#include "core/logger.h"
#include "core/types.h"
#include "systems/audio.h"

#include <miniaudio.h>

#define MAX_TRACKS 64

static struct {
    ma_engine engine;
    ma_sound tracks[MAX_TRACKS];
    u32 track_count;
} local;

bool audio_init() {
    ma_result result = ma_engine_init(NULL, &local.engine);

    if (result == MA_SUCCESS) {
        local.track_count = 0;
        return true;
    } else {
        return false;
    }
}

void audio_deinit() { // TODO: ensure this works with failed initialization
    for (u32 i = 0; i < local.track_count; i++)
        ma_sound_uninit(&local.tracks[i]);
    ma_engine_uninit(&local.engine);
}

u32 audio_load(const char *path) {
    if (local.track_count == MAX_TRACKS) return -1;
    ma_result result = ma_sound_init_from_file(&local.engine, path, 0, NULL, NULL, &local.tracks[local.track_count]);
    if (result != MA_SUCCESS) return -1;
    return local.track_count++;
}

void audio_remove(u32 id) {
    ma_sound *sound = &local.tracks[id];
    if (!sound) return;
    if (ma_sound_is_playing(sound)) ma_sound_stop(sound);
    ma_sound_uninit(sound);
    local.track_count--;
}

void audio_play(u32 id) {
    if (id < 0 || id >= local.track_count) return;
    ma_sound *sound = &local.tracks[id];
    ma_sound_set_volume(sound, 0.4);
    ma_sound_start(sound);
}
