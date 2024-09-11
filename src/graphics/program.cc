#include "core/logger.h"
#include "core/types.h"
#include "graphics/program.h"

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

void program_use(Program *program) {
    glUseProgram(program->handle);
}

void program_destory(Program *program) {
    glDeleteProgram(program->handle);
    for (usize i = 0; i < program->shader_count; i++)
        glDeleteShader(program->shaders[i]);
    free(program->shaders);
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
