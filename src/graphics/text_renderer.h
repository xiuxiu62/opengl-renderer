#pragma once

#include "core/logger.h"
#include "core/types.h"
#include "graphics/program.h"
#include "graphics/texture.h"
#include "math/vector.h"

#include <freetype/freetype.h>
#include <stb_truetype.h>

#define MAX_CHARACTERS 128
#define ATLAS_WIDTH 16
#define ATLAS_HEIGHT 8

struct FreeTypeContext {
    FT_Library lib;
    FT_Face face;
};

struct Character {
    u32 texture_handle;
    i32 advance;
    Vec2 size, bearing;
};

struct TextRenderer {
    Program program;
    u32 vertex_array, vertex_buffer;
    FreeTypeContext ctx;
    static u32 texture_handles[128];
    static Character characters[MAX_CHARACTERS];
};

static TextRenderer text_sample_init() {
    TextRenderer renderer{0};
    FreeTypeContext &ctx = renderer.ctx;
    FT_Face face = renderer.ctx.face;
    u32 *texture_handles = renderer.texture_handles;

    // Initialize freetype library
    if (FT_Init_FreeType(&ctx.lib)) error("Failed to initialize freetype");
    // Load font face
    if (FT_New_Face(ctx.lib, "assets/fonts/Iosevka/Regular.ttc", 0, &ctx.face)) error("Failed to load font");
    // Set font size
    FT_Set_Pixel_Sizes(ctx.face, 0, 48);

    static constexpr usize shader_count = 2;
    ShaderSource shader_sources[shader_count];
    shader_sources[0] = shader_source_load("assets/shaders/text.vert", VERT);
    shader_sources[1] = shader_source_load("assets/shaders/text.frag", FRAG);
    renderer.program = program_create(shader_sources, shader_count);

    // generate glyph map
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(128, texture_handles);

    for (u8 c = 0; c < 128; c++) {
        // load glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            error("Failed to load glyph for character %c", c);
            continue;
        }

        // generate texture
        glBindTexture(GL_TEXTURE_2D, texture_handles[c]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
                     GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // insert the character
        renderer.characters[c] = {
            .texture_handle = texture_handles[c],
            .advance = face->glyph->advance.x,
            .size = {static_cast<f32>(face->glyph->bitmap.width), static_cast<f32>(face->glyph->bitmap.rows)},
            .bearing = {static_cast<f32>(face->glyph->bitmap_left), static_cast<f32>(face->glyph->bitmap_top)},
        };
    }

    glGenVertexArrays(1, &renderer.vertex_array);
    glGenBuffers(1, &renderer.vertex_buffer);

    glBindVertexArray(renderer.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return renderer;
}

static void text_sample_deinit(TextRenderer &renderer) {
    program_destory(renderer.program);
    glDeleteTextures(128, renderer.texture_handles);
    glDeleteBuffers(1, &renderer.vertex_buffer);
    glDeleteVertexArrays(1, &renderer.vertex_array);

    FT_Done_Face(renderer.ctx.face);
    FT_Done_FreeType(renderer.ctx.lib);
}

static void text_sample_draw(TextRenderer &renderer, const char *sample_text) {
    program_use(renderer.program);
    program_set(renderer.program, "text_color", Vec3{1.0f, 0.5f, 0.0f});
}

// TextRenderer text_renderer_init(const char *font_path, f32 font_size);
// void text_renderer_deinit(TextRenderer &text_renderer);

// static void text_sample_font_face_init(TextRenderer &renderer) {
//     FT_Face face = renderer.ctx.face;
//     u32 texture_handles[128];

//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//     glGenTextures(128, texture_handles);

//     for (u8 c = 0; c < 128; c++) {
//         // load glyph
//         if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
//             error("Failed to load glyph for character %c", c);
//             continue;
//         }

//         // generate texture
//         glBindTexture(GL_TEXTURE_2D, texture_handles[c]);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
//                      GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//         // set texture options
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         // insert the character
//         renderer.characters[c] = {
//             .texture_handle = texture_handles[c],
//             .advance = face->glyph->advance.x,
//             .size = {static_cast<f32>(face->glyph->bitmap.width), static_cast<f32>(face->glyph->bitmap.rows)},
//             .bearing = {static_cast<f32>(face->glyph->bitmap_left), static_cast<f32>(face->glyph->bitmap_top)},
//         };
//     }
// }
