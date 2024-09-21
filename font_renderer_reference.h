// text_renderer.h
#pragma once

#include "core/types.h"
#include "graphics/program.h"
#include "math/vector.h"
#include "resources/shader.h"
#include "stb_truetype.h"

#define MAX_CHARACTERS 128
#define ATLAS_WIDTH 16
#define ATLAS_HEIGHT 8

typedef struct {
    Vec2 uv_offset;
    Vec2 size;
    Vec2 bearing;
    u32 advance;
} Character;

typedef struct {
    stbtt_fontinfo font_info;
    u8 *font_buffer;
    Character characters[MAX_CHARACTERS];
    u32 vao, vbo;
    u32 atlas_texture;
    Program program;
    Vec2 atlas_cell_size;
} TextRenderer;

TextRenderer text_renderer_create(const char *font_path, f32 font_size);
void text_renderer_destroy(TextRenderer *renderer);
void text_renderer_render_text(TextRenderer *renderer, const char *text, f32 x, f32 y, f32 scale, Vec3 color);

// text_renderer.c
#include "core/logger.h"
#include "text_renderer.h"
#include <cstdlib>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static u8 *load_font_file(const char *font_path, long *size) {
    // ... (same as before)
}

static void create_font_atlas(TextRenderer *renderer, f32 font_size) {
    f32 scale = stbtt_ScaleForPixelHeight(&renderer->font_info, font_size);

    int atlas_width = ATLAS_WIDTH * (int)font_size;
    int atlas_height = ATLAS_HEIGHT * (int)font_size;
    renderer->atlas_cell_size = (Vec2){font_size, font_size};

    u8 *atlas_bitmap = (u8 *)calloc(atlas_width * atlas_height, sizeof(u8));

    for (u32 c = 0; c < MAX_CHARACTERS; c++) {
        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(&renderer->font_info, c, scale, scale, &x0, &y0, &x1, &y1);

        int char_width = x1 - x0;
        int char_height = y1 - y0;

        int atlas_x = (c % ATLAS_WIDTH) * (int)font_size;
        int atlas_y = (c / ATLAS_WIDTH) * (int)font_size;

        stbtt_MakeCodepointBitmap(&renderer->font_info, atlas_bitmap + atlas_x + atlas_y * atlas_width, char_width,
                                  char_height, atlas_width, scale, scale, c);

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&renderer->font_info, c, &advance, &lsb);

        renderer->characters[c] = (Character){.uv_offset = {(f32)atlas_x / atlas_width, (f32)atlas_y / atlas_height},
                                              .size = {(f32)char_width, (f32)char_height},
                                              .bearing = {(f32)x0, (f32)-y0},
                                              .advance = (u32)(advance * scale)};
    }

    glGenTextures(1, &renderer->atlas_texture);
    glBindTexture(GL_TEXTURE_2D, renderer->atlas_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_bitmap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(atlas_bitmap);
}

TextRenderer text_renderer_create(const char *font_path, f32 font_size) {
    TextRenderer renderer = {0};
    long font_size_bytes;
    renderer.font_buffer = load_font_file(font_path, &font_size_bytes);

    if (!renderer.font_buffer) {
        error("Failed to load font file");
        return renderer;
    }

    if (!stbtt_InitFont(&renderer.font_info, renderer.font_buffer, 0)) {
        error("Failed to initialize font");
        free(renderer.font_buffer);
        return renderer;
    }

    create_font_atlas(&renderer, font_size);

    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders
    ShaderSource vertex_shader = shader_source_load("assets/shaders/text.vert", VERT);
    ShaderSource fragment_shader = shader_source_load("assets/shaders/text.frag", FRAG);
    ShaderSource shaders[] = {vertex_shader, fragment_shader};
    renderer.program = program_create(shaders, 2);

    shader_source_free(&vertex_shader);
    shader_source_free(&fragment_shader);

    return renderer;
}

void text_renderer_destroy(TextRenderer *renderer) {
    glDeleteTextures(1, &renderer->atlas_texture);
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);
    program_destory(&renderer->program);
    free(renderer->font_buffer);
}

void text_renderer_render_text(TextRenderer *renderer, const char *text, f32 x, f32 y, f32 scale, Vec3 color) {
    program_use(&renderer->program);
    glUniform3f(glGetUniformLocation(renderer->program.handle, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->atlas_texture);
    glBindVertexArray(renderer->vao);

    for (const char *p = text; *p; p++) {
        Character ch = renderer->characters[(int)*p];

        f32 xpos = x + ch.bearing.x * scale;
        f32 ypos = y - (ch.size.y - ch.bearing.y) * scale;

        f32 w = ch.size.x * scale;
        f32 h = ch.size.y * scale;

        f32 uv_x = ch.uv_offset.x;
        f32 uv_y = ch.uv_offset.y;
        f32 uv_w = ch.size.x / (renderer->atlas_cell_size.x * ATLAS_WIDTH);
        f32 uv_h = ch.size.y / (renderer->atlas_cell_size.y * ATLAS_HEIGHT);

        f32 vertices[6][4] = {{xpos, ypos + h, uv_x, uv_y},
                              {xpos, ypos, uv_x, uv_y + uv_h},
                              {xpos + w, ypos, uv_x + uv_w, uv_y + uv_h},
                              {xpos, ypos + h, uv_x, uv_y},
                              {xpos + w, ypos, uv_x + uv_w, uv_y + uv_h},
                              {xpos + w, ypos + h, uv_x + uv_w, uv_y}};

        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
