#include "renderer.h"
#include "timer.hpp"
#include <memory>

const bool DEBUG = true;
float CLEAR_COLOR[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// clang-format off
const std::vector<float> VERTICES = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

const unsigned int max_vertex_attributes();

static std::vector<unsigned int> INDICES = { 
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
// clang-format on

int main() {
  // static_assert(VERTICES.size() % 3 == 0, "Invalid number of vertices");
  // static_assert(INDICES.size() % 3 == 0, "Invalid number of indices");

  std::unique_ptr<Renderer> renderer =
      std::make_unique<Renderer>(VERTICES, INDICES, DEBUG);
  renderer->initialize(CLEAR_COLOR);

  if (DEBUG)
    std::cout << "Max vertex attributes: " << max_vertex_attributes()
              << std::endl;

  return renderer->run();
}

const unsigned int max_vertex_attributes() {
  int attribute_count;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribute_count);

  return attribute_count;
}
