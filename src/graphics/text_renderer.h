#pragma once

#include "camera.h"
#include "core/types.h"
#include "graphics/program.h"
#include "math/vector.h"

#include <freetype/freetype.h>

#define MAX_CHARACTERS 128

struct FreeTypeContext {
    FT_Library lib;
    FT_Face face;
};

struct Character {
    u32 texture_handle;
    i32 advance;
    Vec2 size, bearing;

    struct Vertex {
        Vec2 position, uv;
    };
};

struct TextRenderer {
    Program program;
    u32 vertex_array, vertex_buffer;
    FreeTypeContext ctx;
    u32 texture_handles[MAX_CHARACTERS];
    Character characters[MAX_CHARACTERS];
};

void text_renderer_init(void);
void text_renderer_deinit(void);
void text_renderer_begin(Camera &camera);
void text_renderer_draw(Vec2 pos, f32 scale, const char *message);
void text_renderer_end(void);
