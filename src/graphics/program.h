#pragma once

#include "core/types.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "resources/shader.h"

struct Program {
    u32 handle;
    u32 *shaders;
    usize shader_count;
};

Program program_create(ShaderSource *sources, usize source_count);
void program_destory(Program &program);
void program_use(Program &program);
u32 program_locate(Program &program, const char *uniform_name);
void program_set(Program &program, i32 value);
void program_set(Program &program, const char *uniform_name, i32 value);
void program_set(Program &program, u32 location, f32 value);
void program_set(Program &program, const char *uniform_name, f32 value);
void program_set(Program &program, u32 location, Vec2 value);
void program_set(Program &program, const char *uniform_name, Vec2 value);
void program_set(Program &program, u32 location, Vec3 value);
void program_set(Program &program, const char *uniform_name, Vec3 value);
void program_set(Program &program, u32 location, Vec4 value);
void program_set(Program &program, const char *uniform_name, Vec4 value);
void program_set(Program &program, u32 location, Mat3 *value, usize count = 1);
void program_set(Program &program, const char *uniform_name, Mat3 *value, usize count = 1);
void program_set(Program &program, u32 location, Mat4 *value, usize count = 1);
void program_set(Program &program, const char *uniform_name, Mat4 *value, usize count = 1);
