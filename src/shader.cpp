#include "shader.hpp"
#include "fs.hpp"
#include <GL/glew.h>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

const size_t COMPILE_ERROR_LOG_SIZE = 1024;

int check_compilation(unsigned int id, unsigned int *compile_status,
                      char *buffer);
int check_compilation(std::vector<unsigned int> ids,
                      unsigned int *compile_status, char *buffer);

Shader::Shader(std::string name, unsigned int *compile_status,
               const char *vertex_data) {
  unsigned int vertex_id;
  char buffer[COMPILE_ERROR_LOG_SIZE];

  vertex_id = glCreateShader(GL_VERTEX_SHADER);
  this->name = name;

  glShaderSource(vertex_id, 1, &vertex_data, NULL);
  glCompileShader(vertex_id);
  if (!check_compilation(vertex_id, compile_status, buffer))
    return;

  this->_id = glCreateProgram();
  glAttachShader(this->_id, vertex_id);
  glLinkProgram(this->_id);

  glDeleteShader(vertex_id);
}

Shader::Shader(std::string name, unsigned int *compile_status,
               const char *vertex_data, const char *fragment_data) {
  unsigned int vertex_id, fragment_id;
  char buffer[COMPILE_ERROR_LOG_SIZE];

  this->name = name;

  vertex_id = glCreateShader(GL_VERTEX_SHADER);
  fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertex_id, 1, &vertex_data, NULL);
  glCompileShader(vertex_id);

  glShaderSource(fragment_id, 1, &fragment_data, NULL);
  glCompileShader(fragment_id);

  if (!check_compilation({vertex_id, fragment_id}, compile_status, buffer))
    return;

  this->_id = glCreateProgram();
  glAttachShader(this->_id, vertex_id);
  glAttachShader(this->_id, fragment_id);
  glLinkProgram(this->_id);

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
}

std::optional<std::unique_ptr<Shader>> Shader::load(const char *vertex_path) {
  unsigned int compile_status;
  auto name = std::filesystem::path(vertex_path).stem().string();
  std::optional<std::string> vertex_data =
      core::file::read_to_string(vertex_path);

  if (!vertex_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  Shader *shader =
      new Shader(name, &compile_status, vertex_data.value().c_str());
  if (!compile_status)
    return std::nullopt;

  return std::unique_ptr<Shader>(shader);
}

std::optional<std::unique_ptr<Shader>> Shader::load(const char *vertex_path,
                                                    const char *fragment_path) {
  unsigned int compile_status;
  std::string name;
  std::optional<std::string> vertex_data, fragment_data;

  name = std::filesystem::path(vertex_path).stem().string();

  vertex_data = core::file::read_to_string(vertex_path);
  if (!vertex_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  fragment_data = core::file::read_to_string(vertex_path);
  if (!fragment_data.has_value()) {
    std::cout << "Failed to load shader: " << name << std::endl;
    return std::nullopt;
  }

  Shader *shader =
      new Shader(name, &compile_status, vertex_data.value().c_str(),
                 fragment_data.value().c_str());
  if (!compile_status)
    return std::nullopt;

  return std::unique_ptr<Shader>(shader);
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

int check_compilation(unsigned int id, unsigned int *compile_status,
                      char *buffer) {
  GLint is_compiled;
  glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_TRUE)
    return 0;

  glGetShaderInfoLog(id, COMPILE_ERROR_LOG_SIZE, 0, buffer);
  std::cout << "Failed to compile shader:" << std::endl << buffer << std::endl;

  return -1;
}

int check_compilation(std::vector<unsigned int> ids,
                      unsigned int *compile_status, char *buffer) {
  GLint is_compiled;
  for (auto &id : ids) {
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_TRUE)
      continue;

    glGetShaderInfoLog(id, COMPILE_ERROR_LOG_SIZE, 0, buffer);
    std::cout << "Failed to compile shader:" << std::endl
              << buffer << std::endl;

    return -1;
  }

  return 0;
}
