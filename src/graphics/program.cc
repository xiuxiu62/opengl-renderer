#include "core/debug.h"
#include "core/logger.h"
#include "core/types.h"
#include "graphics/program.h"
#include "math/matrix.h"

#include <cstdlib>
#include <cstring>

u32 shader_create(ShaderSource src, i32 *success, char info_buffer[512]);

Program program_create(ShaderSource *sources, usize source_count) {
    Program program{
        .handle = glCreateProgram(),
        .shaders = static_cast<u32 *>(calloc(source_count, sizeof(u32))),
        .shader_count = source_count,
    };

    i32 success;
    char info_buffer[512];

    for (usize i = 0; i < source_count; i++) {
        program.shaders[i] = shader_create(sources[i], &success, info_buffer);
        glAttachShader(program.handle, program.shaders[i]);
    }

    glLinkProgram(program.handle);
    glGetProgramiv(program.handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program.handle, 512, NULL, info_buffer);
        error("%s", info_buffer);
        program.handle = 0;
    }

    return program;
}

void program_destory(Program &program) {
    glDeleteProgram(program.handle);
    for (usize i = 0; i < program.shader_count; i++)
        glDeleteShader(program.shaders[i]);
    free(program.shaders);
}

void program_use(Program &program) {
    glUseProgram(program.handle);
}

u32 program_locate(Program &program, const char *uniform_name) {
    // when_debug({
    //     auto loc = glGetUniformLocation(program.handle, uniform_name);
    //     info("%d", loc);
    //     return loc;
    // });
    return glGetUniformLocation(program.handle, uniform_name);
}

void program_set(Program &program, u32 location, i32 value) {
    glUniform1i(location, value);
}

void program_set(Program &program, const char *uniform_name, i32 value) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, f32 value) {
    glUniform1f(location, value);
}

void program_set(Program &program, const char *uniform_name, f32 value) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, Vec2 value) {
    glUniform2f(location, value.x, value.y);
}

void program_set(Program &program, const char *uniform_name, Vec2 value) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, Vec3 value) {
    glUniform3f(location, value.x, value.y, value.z);
}

void program_set(Program &program, const char *uniform_name, Vec3 value) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, Vec4 value) {
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void program_set(Program &program, const char *uniform_name, Vec4 value) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, Mat3 *value, usize count) {
    glUniformMatrix3fv(location, count, false, reinterpret_cast<f32 *>(value));
}

void program_set(Program &program, const char *uniform_name, Mat3 *value, usize count) {
    program_set(program, program_locate(program, uniform_name), value);
}

void program_set(Program &program, u32 location, Mat4 *value, usize count) {
    glUniformMatrix4fv(location, count, false, reinterpret_cast<f32 *>(value));
}

void program_set(Program &program, const char *uniform_name, Mat4 *value, usize count) {
    program_set(program, program_locate(program, uniform_name), value);
}

static i32 shader_kind_to_gl(ShaderKind kind) {
    switch (kind) {
    case VERT:
        return GL_VERTEX_SHADER;
    case TESC:
        return GL_TESS_CONTROL_SHADER;
    case TESE:
        return GL_TESS_EVALUATION_SHADER;
    case GEOM:
        return GL_GEOMETRY_SHADER;
    case FRAG:
        return GL_FRAGMENT_SHADER;
    case COMP:
        return GL_COMPUTE_SHADER;
    default:
        unreachable();
    }
}

u32 shader_create(ShaderSource src, i32 *success, char info_buffer[512]) {
    u32 handle = glCreateShader(shader_kind_to_gl(src.kind));
    glShaderSource(handle, 1, &src.code, nullptr);
    glCompileShader(handle);

    glGetShaderiv(handle, GL_COMPILE_STATUS, success);
    if (!*success) {
        memset(info_buffer, 0, 512);
        glGetShaderInfoLog(handle, 512, NULL, info_buffer);
        error("%s", info_buffer);
        return 0;
    };

    return handle;
}
