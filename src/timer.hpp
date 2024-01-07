#include <GLFW/glfw3.h>
#include <memory>

namespace core::time {
struct Timer;

static std::unique_ptr<Timer> global_timer = std::make_unique<Timer>();

// namespace time {
struct Timer {
private:
  float current_frame;
  float last_frame;
  float delta_t_inner;

public:
  Timer() : current_frame(0.0), last_frame(0.0), delta_t_inner(0.0) {}
  const void update();
  const inline float delta_t() const;
};
} // namespace core::time
