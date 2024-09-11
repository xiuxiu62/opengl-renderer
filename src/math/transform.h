#pragma once

#include "math/matrix.h"
#include "math/rotor.h"
#include "math/vector.h"

struct Transform {
    Vec2 position, scale;
    Rot2 rotation;

    Mat4 to_mat4();
};
