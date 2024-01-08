#include "shader.hpp"
#include "fs.hpp"
#include "glad.hpp"
#include <filesystem>
#include <iostream>

Shader::Shader(std::string name, const char *vertex_data) {
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  this->name = name;

  glShaderSource(vertex, 1, &vertex_data, NULL);
  glCompileShader(vertex);

  this->_id = glCreateProgram();
  glAttachShader(this->_id, vertex);
  glLinkProgram(this->_id);

  glDeleteShader(vertex);
}

Shader::Shader(std::string name, const char *vertex_data,
               const char *fragment_data) {
  this->name = name;

  unsigned int vertex, fragment;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertex, 1, &vertex_data, NULL);
  glCompileShader(vertex);

  glShaderSource(fragment, 1, &fragment_data, NULL);
  glCompileShader(fragment);

  this->_id = glCreateProgram();
  glAttachShader(this->_id, vertex);
  glAttachShader(this->_id, fragment);
  glLinkProgram(this->_id);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

std::optional<Shader> Shader::load(const char *vertex_path) {
  auto name = std::filesystem::path(vertex_path).stem().string();
  std::optional<std::string> vertex_data =
      core::file::read_to_string(vertex_path);

  if (!vertex_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  return Shader(name, vertex_data.value().c_str());
}

std::optional<Shader> Shader::load(const char *vertex_path,
                                   const char *fragment_path) {
  std::string name;
  std::optional<std::string> vertex_data, fragment_data;

  name = std::filesystem::path(vertex_path).stem().string();

  vertex_data = core::file::read_to_string(vertex_path);
  if (!vertex_data.has_value() || !fragment_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  fragment_data = core::file::read_to_string(vertex_path);
  if (!fragment_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  return Shader(name, vertex_data.value().c_str(),
                fragment_data.value().c_str());
}

void Shader::use() const { glUseProgram(this->_id); }

unsigned int Shader::id() const { return this->_id; }

void Shader::set_bool(const std::string &name, const bool &value) const {
  glUniform1i(glGetUniformLocation(this->_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, const unsigned &value) const {
  glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, const float &value) const {
  glUniform1f(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void Shader::set_mat2(const std::string &name, const glm::mat2 &value) const {
  glUniformMatrix2fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void Shader::set_mat3(const std::string &name, const glm::mat3 &value) const {
  glUniformMatrix3fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}
