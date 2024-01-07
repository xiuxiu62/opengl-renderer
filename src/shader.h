#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

struct Shader {
  std::string name;
  std::string vertex_source;
  std::optional<std::string> fragment_source;

  bool is_compiled;
  unsigned int program;
  unsigned int vertex_shader;
  std::optional<unsigned int> fragment_shader;

  Shader(std::string name, std::string vertex_source,
         std::optional<std::string> fragment_source)
      : name(std::move(name)), vertex_source(std::move(vertex_source)),
        fragment_source(std::move(fragment_source)) {
    this->is_compiled = false;
  }

  ~Shader();

  static std::optional<Shader>
  load(const std::string vertex_path,
       const std::optional<std::string> fragment_path);

  void compile(bool debug);
  void use();
};
