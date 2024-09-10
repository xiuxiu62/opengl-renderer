#pragma once

#include "math/rotor.h"
#include "math/vector.h"

struct Transform {
    Vec2 position;
    Vec2 scale;
    Rot2 rotation;
};
