#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 looking_at, glm::vec3 up)
    : position(std::move(position)), looking_at(std::move(looking_at)),
      up(std::move(up)) {
  this->direction = glm::normalize(this->position - this->looking_at);
  this->right_axis = glm::normalize(glm::cross(this->up, this->direction));
  this->up_axis = glm::cross(this->direction, this->right_axis);
  this->view = glm::lookAt(this->position, this->looking_at, this->up);
}

void Camera::update(float delta_t) {
  const float radius = 10.0f;
  float cam_x = sin(glfwGetTime()) * radius;
  float cam_z = cos(glfwGetTime()) * radius;

  this->view =
      glm::lookAt(glm::vec3(cam_x, 0.0, cam_z), this->looking_at, this->up);
}
