#pragma once

#include "core/types.h"
#include "resources/shader.h"

struct Program {
    u32 handle;
    u32 *shaders;
    usize shader_count;
};

Program program_create(ShaderSource *sources, usize source_count);
void program_destory(Program &program);
void program_use(Program &program);
