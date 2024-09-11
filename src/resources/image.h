#pragma once

#include "collections/generational_arena.h"
#include "core/types.h"

struct Image {
    u32 width, height, channel_count;
    u8 *data;
};

void image_manager_init();

void image_manager_deinit();

GenHandle image_load(const char *path);

Image *image_get(GenHandle handle);

bool image_remove(GenHandle handle);
