#include "timer.hpp"

namespace core::time {
void Timer::update() {
  this->current_frame = glfwGetTime();
  this->_delta_t = this->current_frame - this->last_frame;
  this->last_frame = current_frame;
}

inline float Timer::delta_t() const { return this->_delta_t; }
} // namespace core::time
