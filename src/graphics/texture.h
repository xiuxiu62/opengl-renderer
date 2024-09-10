#pragma once

#include "resources/image.h"
#include "types.h"

struct Texture {
    u32 handle;
    GenHandle image_handle;
    u32 format;
};

Texture texture_create(GenHandle image_handle);

void texture_destroy(Texture texture);

void texture_use(Texture texture);
