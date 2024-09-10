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
