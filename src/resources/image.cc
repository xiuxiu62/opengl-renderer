#include "resources/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static Image::Manager manager;

void image_manager_init() {
    manager = gen_arena_create(sizeof(Image));
}

void image_manager_deinit() {
    gen_arena_destroy(&manager);
}

GenHandle image_load(const char *path) {
    Image image;
    image.data = stbi_load(path, reinterpret_cast<i32 *>(&image.width), reinterpret_cast<i32 *>(&image.height),
                           reinterpret_cast<i32 *>(&image.channel_cnt), 0);
    return gen_arena_insert(&manager, reinterpret_cast<u8 *>(&image));
}

bool image_destory(GenHandle handle) {
    Image *image = reinterpret_cast<Image *>(gen_arena_get(&manager, handle));
    if (image) {
        free(image->data);
        return true;
    } else {
        return false;
    }
}
