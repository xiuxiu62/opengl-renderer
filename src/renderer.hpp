#pragma once

// #include "glad.hpp"
#include "shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>

struct MeshObject {
public:
  unsigned int vbo, vao, ebo;

  MeshObject() {}
  MeshObject(unsigned int vbo, unsigned int vao, unsigned int ebo)
      : vbo(vbo), vao(vao), ebo(ebo) {}
};

struct Renderer {
private:
  GLFWwindow *window;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  bool debug;

  void clear();
  std::optional<Shader> load_triangle_shader();
  MeshObject *initialize_mesh();
  void display_mesh(Shader *shader, MeshObject *object);
  void process_input();

public:
  Renderer(std::vector<float> vertices, std::vector<unsigned int> indices,
           bool debug)
      : vertices(std::move(vertices)), indices(std::move(indices)),
        debug(debug) {}

  int initialize(float *clear_color);
  int run();
  void deinitialize(Shader *shader, MeshObject *object);
};
