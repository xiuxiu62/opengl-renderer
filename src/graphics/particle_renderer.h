#include "math/vector.h"

struct Particle {
    Vec2 position, velocity;
    Vec4 color;
    f32 life_time;

    static constexpr Particle DEFAULT() {
        return {
            .position = Vec2::ZERO(),
            .velocity = Vec2::ZERO(),
            .color = Vec4::ONE(),
            .life_time = 0.0,
        };
    }
};
