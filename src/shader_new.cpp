#include "shader_new.hpp"
#include "fs.h"
#include <filesystem>
#include <iostream>

Shader::Shader(const char *vertex_path,
               std::optional<const char *> fragment_path) {
  std::string name;
  std::optional<std::string> vertex, fragment;

  name = std::filesystem::path(vertex_path).stem().string();

  vertex = file::read_to_string(vertex_path);
  if (!vertex.has_value()) {
    std::cout << "Failed to read vertex shader: " << vertex_path << std::endl;
  }

  fragment = std::nullopt;
  if (fragment_path.has_value())
    fragment = file::read_to_string(fragment_path.value());

  // glShaderSource(vertex, 1, &vShaderCode, NULL);
  // glCompileShader(vertex);
  // checkCompileErrors(vertex, "VERTEX");
  // // fragment Shader
  // fragment = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fragment, 1, &fShaderCode, NULL);
  // glCompileShader(fragment);
  // checkCompileErrors(fragment, "FRAGMENT");
  // // shader Program
  // ID = glCreateProgram();
  // glAttachShader(ID, vertex);
  // glAttachShader(ID, fragment);
  // glLinkProgram(ID);
  // checkCompileErrors(ID, "PROGRAM");
  // // delete the shaders as they're linked into our program now and no longer
  // // necessary
  // glDeleteShader(vertex);
  // glDeleteShader(fragment);

  // return Shader(name, vertex.value(), fragment);
}
