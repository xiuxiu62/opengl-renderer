#pragma once

#include "core/types.h"
#include "resources/image.h"

struct Texture {
    u32 handle;
    GenHandle image_handle;
    u32 format;
};

Texture texture_create(GenHandle image_handle);

void texture_destroy(Texture texture);

void texture_use(Texture texture);
