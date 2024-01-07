#include <glm/glm.hpp>

struct Camera {
  glm::vec3 position;
  glm::vec3 looking_at;
  glm::vec3 direction;
  glm::vec3 up;

  glm::vec3 up_axis;
  glm::vec3 right_axis;
  glm::mat4 view;

  Camera(glm::vec3 position, glm::vec3 looking_at, glm::vec3 up);
  void update(float delta_t);
};
