#pragma once

#include "types.h"

#include <cmath>

namespace math {
constexpr f32 clamp(f32 value, f32 min, f32 max) {
    return std::fmax(min, std::fmin(value, max));
}
} // namespace math
