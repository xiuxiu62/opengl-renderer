#include "graphics/text_renderer.h"

#include "core/logger.h"
#include "freetype/freetype.h"
#include "freetype/fttypes.h"
#include "graphics/program.h"
#include "graphics/texture.h"

#include <cstring>

#define MAX_CHARACTERS 128

static TextRenderer renderer;

void text_renderer_init(void) {
    FreeTypeContext &ctx = renderer.ctx;
    FT_Face &face = renderer.ctx.face;
    u32 *texture_handles = renderer.texture_handles;

    // Initialize freetype library
    FT_Error ft_err = FT_Init_FreeType(&ctx.lib);
    if (ft_err) {
        error("Failed to initialize freetype");
        return;
    };

    // Load font face
    const char *font_path = "assets/fonts/Roboto/Roboto-Regular.ttf";
    ft_err = FT_New_Face(ctx.lib, font_path, 0, &ctx.face);
    if (ft_err) {
        error("Failed to load font");
        return;
    }

    // Set font size
    ft_err = FT_Set_Pixel_Sizes(ctx.face, 0, 48);
    if (ft_err) {
        error("Failed to set font size");
        return;
    }

    static constexpr usize shader_count = 2;
    ShaderSource shader_sources[shader_count];
    shader_sources[0] = shader_source_load("assets/shaders/text.vert", VERT);
    shader_sources[1] = shader_source_load("assets/shaders/text.frag", FRAG);
    renderer.program = program_create(shader_sources, shader_count);
    // program_use(renderer.program);

    // generate glyph map
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(MAX_CHARACTERS, texture_handles);

    for (u8 c = 0; c < MAX_CHARACTERS; c++) {
        u32 &texture = texture_handles[c];

        // load glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            error("Failed to load glyph for character %c", static_cast<char>(c));
            continue;
        }

        // glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // generate texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
                     GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // insert the character
        renderer.characters[c] = {
            .texture_handle = texture,
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
}

void text_renderer_deinit(void) {
    program_destory(renderer.program);
    glDeleteTextures(MAX_CHARACTERS, renderer.texture_handles);
    glDeleteBuffers(1, &renderer.vertex_buffer);
    glDeleteVertexArrays(1, &renderer.vertex_array);

    FT_Done_Face(renderer.ctx.face);
    FT_Done_FreeType(renderer.ctx.lib);
}

void text_renderer_begin(void) {
    program_use(renderer.program);
    program_set(renderer.program, "text_color", Vec3{1.0f, 0.5f, 0.0f});
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(renderer.vertex_array);
}

void text_renderer_draw(Vec2 pos, f32 scale, const char *message, usize message_len) {
    for (usize i = 0; i < message_len; i++) {
        char raw = message[i];
        Character c = renderer.characters[raw];

        Vec2 char_pos = pos + Vec2{c.bearing.x * scale, -(c.size.y - c.bearing.y) * scale};
        f32 w = c.size.x * scale, h = c.size.y * scale;

        Character::Vertex vertices[6]{
            {char_pos.x, char_pos.y + h, 0.0f, 0.0f},
            {.position = {char_pos.x, char_pos.y}, .uv = {0.0f, 1.0f}},
            {.position = {char_pos.x + w, char_pos.y}, .uv = {1.0f, 1.0f}},

            {.position = {char_pos.x, char_pos.y + h}, .uv = {0.0f, 0.0f}},
            {.position = {char_pos.x + w, char_pos.y}, .uv = {1.0f, 1.0f}},
            {.position = {char_pos.x + w, char_pos.y + h}, .uv = {1.0f, 0.0f}},
        };

        glBindTexture(GL_TEXTURE_2D, c.texture_handle);
        glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Character::Vertex), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        char_pos.x += (c.advance >> 6) * scale;
    }
}

void text_renderer_end(void) {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
