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
  float _delta_t;

public:
  Timer() : current_frame(0.0), last_frame(0.0), _delta_t(0.0) {}
  void update();
  inline float delta_t() const;
};
} // namespace core::time
