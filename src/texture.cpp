#include "texture.hpp"
// #include "glad.hpp"

#include "image.hpp"
#include <GL/glew.h>
#include <iostream>
#include <optional>

Texture::Texture(unsigned char *data, int width, int height, int channels) {
  glGenTextures(1, &this->id);
  this->bind();

  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned int format = (channels == 3) ? GL_RGB : GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  this->unbind();
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }

std::optional<Texture> Texture::load(const char *path) {
  int width, height, channels;
  unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

  if (!data) {
    std::cout << "Failed to load texture: " << path << std::endl;
    return std::nullopt;
  }

  Texture self = Texture(data, width, height, channels);
  stbi_image_free(data);

  return self;
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, this->id); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
