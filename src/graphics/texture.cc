#include "graphics/texture.h"

Texture texture_create(GenHandle image_handle) {
    Texture texture{.image_handle = image_handle};
    Image *image = image_get(image_handle);

    glGenTextures(1, &texture.handle);
    glBindTexture(GL_TEXTURE_2D, texture.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture.format = (image->channel_cnt == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, texture.format, image->width, image->height, 0, texture.format, GL_UNSIGNED_BYTE,
                 image->data);
    // glGenerateMipmap(GL_TEXTURE_2D); // TODO: determine if this is helpful

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void texture_destroy(Texture texture) {
    glDeleteTextures(1, &texture.handle);
}

void texture_use(Texture texture) {
    glActiveTexture(texture.handle);
    glBindTexture(GL_TEXTURE_2D, texture.handle);
}
