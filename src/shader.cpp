#include "shader.h"
#include "fs.h"

#include "glad.h"
#include <filesystem>
#include <iostream>
#include <optional>

Shader::~Shader() { glDeleteProgram(this->program); }

std::optional<Shader>
Shader::load(const std::string vertex_path,
             const std::optional<std::string> fragment_path) {
  std::string name;
  std::optional<std::string> vertex, fragment;

  name = std::filesystem::path(vertex_path).stem().string();

  vertex = file::read_to_string(vertex_path);
  if (!vertex.has_value()) {
    std::cout << "Failed to read vertex shader: " << vertex_path << std::endl;

    return std::nullopt;
  }

  fragment = std::nullopt;
  if (fragment_path.has_value())
    fragment = file::read_to_string(fragment_path.value());

  return Shader(name, vertex.value(), fragment);
}

void Shader::compile(bool debug) {
  int success;
  const int info_buffer_size = 512;
  char info_buffer[info_buffer_size];
  const char *source;

  this->program = glCreateProgram();

  // Compile vertex shader
  this->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  source = this->vertex_source.c_str();

  glShaderSource(this->vertex_shader, 1, &source, NULL);
  glCompileShader(this->vertex_shader);
  glGetShaderiv(this->vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(this->vertex_shader, info_buffer_size, NULL,
                       info_buffer);
    std::cout << "Failed to compile vertex shader:" << std::endl
              << info_buffer << std::endl;
  }

  glAttachShader(this->program, this->vertex_shader);

  // Compile fragment shader
  if (this->fragment_source.has_value()) {
    this->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    source = this->fragment_source.value().c_str();

    glShaderSource(this->fragment_shader.value(), 1, &source, NULL);
    glCompileShader(this->fragment_shader.value());
    glGetShaderiv(this->fragment_shader.value(), GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(this->vertex_shader, info_buffer_size, NULL,
                         info_buffer);
      std::cout << "Failed to compile fragment shader:" << std::endl
                << info_buffer << std::endl;
    }

    glAttachShader(this->program, this->fragment_shader.value());
  }

  // Link program
  glLinkProgram(this->program);
  glGetProgramiv(this->program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(this->vertex_shader, info_buffer_size, NULL,
                       info_buffer);
    std::cout << "Failed to link program:" << std::endl
              << info_buffer << std::endl;
  }

  // Clean up
  glDeleteShader(this->vertex_shader);
  if (this->fragment_shader.has_value())
    glDeleteShader(this->vertex_shader);

  this->is_compiled = true;

  if (debug)
    std::cout << "[Compiled shader] " << this->name << std::endl;
}

void Shader::use() { glUseProgram(this->program); }
