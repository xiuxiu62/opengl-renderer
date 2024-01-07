#include "timer.hpp"

namespace core::time {
const void Timer::update() {
  this->current_frame = glfwGetTime();
  this->delta_t_inner = this->current_frame - this->last_frame;
  this->last_frame = current_frame;
}

const inline float Timer::delta_t() const { return this->delta_t_inner; }
} // namespace core::time
