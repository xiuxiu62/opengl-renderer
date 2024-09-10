#pragma once

#include "types.h"

struct Image {
    u32 width, height, channel_cnt;
    u8 *data;
};

Image image_load(const char *path);

void image_destory(Image *image);
