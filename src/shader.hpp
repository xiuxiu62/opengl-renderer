#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>

struct Shader {
private:
  unsigned int _id;
  std::string name;

public:
  Shader(std::string name, unsigned int *compile_status,
         const char *vertex_data);
  Shader(std::string name, unsigned int *compile_status,
         const char *vertex_data, const char *fragment_data);

  static std::optional<std::unique_ptr<Shader>> load(const char *vertex_path);

  static std::optional<std::unique_ptr<Shader>> load(const char *vertex_path,
                                                     const char *fragment_path);

  void use() const;
  unsigned int id() const;
  void set_bool(const std::string &name, const bool &value) const;
  void set_int(const std::string &name, const unsigned &value) const;
  void set_float(const std::string &name, const float &value) const;
  void set_vec2(const std::string &name, const glm::vec2 &value) const;
  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_mat2(const std::string &name, const glm::mat2 &value) const;
  void set_mat3(const std::string &name, const glm::mat3 &value) const;
  void set_mat4(const std::string &name, const glm::mat4 &value) const;
};
