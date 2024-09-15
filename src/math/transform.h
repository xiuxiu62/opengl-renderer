#pragma once

#include "math/matrix.h"
#include "math/rotor.h"
#include "math/vector.h"

struct Transform {
    Vec2 translation, scale;
    Rot2 rotation;

    static Transform from_translation(Vec2 translation);
    static Transform from_scale(Vec2 scale);
    static Transform from_rotation(Rot2 rotation);

    Mat4 to_mat4();
};
