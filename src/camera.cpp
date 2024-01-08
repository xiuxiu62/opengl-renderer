#include "camera.hpp"
#include <glm/ext.hpp>

namespace core::camera {

using namespace glm;

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
    : position(std::move(position)), up(std::move(up)), yaw(yaw), pitch(pitch),
      front(vec3(0.0F, 0.0F, -1.0F)), movement_speed(defaults::SPEED),
      mouse_sensitivity(defaults::SENSITIVITY), world_up(std::move(up)),
      _zoom(defaults::ZOOM) {
  this->update();
}

void Camera::update() {
  vec3 front = {
      cos(radians(this->yaw) * cos(radians(this->pitch))),
      sin(radians(this->pitch)),
      sin(radians(this->yaw) * cos(radians(this->pitch))),
  };

  this->front = normalize(front);
  this->right = normalize(cross(this->front, this->world_up));
  this->up = normalize(cross(this->right, this->front));
}

mat4 Camera::view_projection() {
  return lookAt(this->position, this->position + this->front, this->up);
}

void Camera::move(Direction direction, float delta_t) {
  float velocity = this->movement_speed * delta_t;
  switch (direction) {
  case FORWARD:
    this->position += this->front * velocity;
    break;
  case BACKWARD:
    this->position += this->front * velocity;
    break;
  case UP:
    this->position += this->up * velocity;
    break;
  case DOWN:
    this->position -= this->up * velocity;
    break;
  case RIGHT:
    this->position += this->right * velocity;
    break;
  case LEFT:
    this->position -= this->right * velocity;
    break;
  default:
    break;
  };
}

void Camera::rotate(float x_offset, float y_offset, bool pitch_constrained) {
  this->yaw += x_offset * this->mouse_sensitivity;
  this->pitch += y_offset * this->mouse_sensitivity;

  if (!pitch_constrained) {
    if (this->pitch > 90.0f)
      this->pitch = 90.0f;

    if (this->pitch < -90.0f)
      this->pitch = -90.0f;
  }

  this->update();
}

void Camera::zoom(float y_offset) {
  this->_zoom -= y_offset;

  if (this->_zoom > 45.0f)
    this->_zoom = 45.0f;
  if (this->_zoom < 1.0f)
    this->_zoom = 1.0f;
}
}; // namespace core::camera
