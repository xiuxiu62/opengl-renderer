#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>

struct Shader {
private:
  unsigned int ID;

public:
  Shader(const char *vertex_path, std::optional<const char *> fragment_path);

  inline void use() const;
  void set_bool(const std::string &name, const bool &value) const;
  void set_int(const std::string &name, const unsigned &value) const;
  void set_float(const std::string &name, const float &value) const;
  void set_mat4(const std::string &name, const glm::mat4 &value) const;
};
