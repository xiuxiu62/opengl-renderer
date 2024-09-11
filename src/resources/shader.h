#pragma once

#include "core/types.h"

enum ShaderKind {
    VERT,
    TESC,
    TESE,
    GEOM,
    FRAG,
    COMP,
};

struct ShaderSource {
    ShaderKind kind;
    const char *path;
    char *code;
    usize size;
};

ShaderSource shader_source_load(const char *path, ShaderKind kind);
void shader_source_free(ShaderSource *src);
