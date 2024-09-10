#include "resources/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image image_load(const char *path) {
    Image image;
    image.data = stbi_load(path, reinterpret_cast<i32 *>(&image.width), reinterpret_cast<i32 *>(&image.height),
                           reinterpret_cast<i32 *>(&image.channel_cnt), 0);
    return image;
}

void image_destory(Image *image) {
    free(image->data);
}
