#include <glm/glm.hpp>

namespace core::camera {

enum Direction { FORWARD, BACKWARD, UP, DOWN, LEFT, RIGHT };

namespace defaults {
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
}; // namespace defaults

struct Camera {
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  float yaw;
  float pitch;

  float movement_speed;
  float mouse_sensitivity;
  float _zoom;

public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = defaults::YAW,
         float pitch = defaults::PITCH);

  void update();
  glm::mat4 view_projection();
  void move(Direction direction, float delta_t);
  void rotate(float x_offset, float y_offset, bool pitch_constrained = true);
  void zoom(float y_offset);
};
} // namespace core::camera
