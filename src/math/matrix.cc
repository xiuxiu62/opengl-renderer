#include "math/matrix.h"

Mat3 Mat3::translation(const Vec2 &v) {
    return {
        {1, 0, v.x},
        {0, 1, v.y},
        {0, 0, 1},
    };
}

Mat3 Mat3::rotation(f32 angle) {
    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    return {
        {c, -s, 0},
        {s, c, 0},
        {0, 0, 1},
    };
}

Mat3 scale(const Vec2 &v) {
    return {
        {v.x, 0, 0},
        {0, v.y, 0},
        {0, 0, 1},
    };
}

// Mat4 Mat4::ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
//     Mat4 result;
//     result.m[0][0] = 2.0f / (right - left);
//     result.m[1][1] = 2.0f / (top - bottom);
//     result.m[2][2] = -2.0f / (far - near);
//     result.m[0][3] = -(right + left) / (right - left);
//     result.m[1][3] = -(top + bottom) / (top - bottom);
//     result.m[2][3] = -(far + near) / (far - near);
//     return result;
// }

Mat4 Mat4::ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
    f32 rl = 1.0 / (right - left);
    f32 tb = 1.0 / (top - bottom);
    f32 fn = 1.0 / (far - near);

    return {
        {2.0f * rl, 0.0, 0.0, 0.0},
        {0.0, 2.0f * tb, 0.0, 0.0},
        {0.0, 0.0, 2.0f * fn, 0.0},
        {-(right + left) * rl, -(top + bottom) * tb, -(far + near) * fn, 1.0},
    };
}

Mat4 Mat4::translation(const Vec4 &v) {
    return {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {v.x, v.y, v.z, 1},
    };
}

Mat4 Mat4::scale(const Vec3 &v) {
    return {
        {v.x, 0, 0, 0},
        {0, v.y, 0, 0},
        {0, 0, v.z, 0},
        {0, 0, 0, 1},
    };
}

// Mat4 rotation(f32 angle);

// Mat3x3 Mat3x3::transposed() const {
//     return {
//         {x.x, y.x, z.x},
//         {x.y, y.y, z.y},
//         {x.z, y.z, z.z},
//     };
// }

// f32 Mat3x3::determinant() const {
//     return x.x * (y.y * z.z - y.z * z.y)   //
//            - x.y * (y.x * z.z - y.z * z.x) //
//            + x.z * (y.x * z.y - y.y * z.x);
// }

// Mat3x3 Mat3x3::inverse() const {
//     f32 det = determinant();
//     if (std::fabs(det) < EPSILON) return Mat3x3::IDENTITY;

//     f32 inv_det = 1 / det;
//     return {
//         {
//             (y.y * z.z - y.z * z.y) * inv_det,
//             (x.z * z.y - y.z * z.y) * inv_det,
//             (y.y * z.z - y.z * z.y) * inv_det,
//         },
//     };
// }
