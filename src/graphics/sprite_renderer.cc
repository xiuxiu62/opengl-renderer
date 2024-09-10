#include "graphics/sprite_renderer.h"

SpriteRenderer sprite_renderer_create() {
    SpriteRenderer renderer;

    static constexpr usize shader_count = 2;
    ShaderSource shader_sources[shader_count];
    shader_sources[0] = shader_source_load("assets/shaders/sprite.vert", VERT);
    shader_sources[1] = shader_source_load("assets/shaders/sprite.frag", FRAG);
    renderer.program = program_create(shader_sources, shader_count);

    u32 buffers[2];

    glGenBuffers(2, buffers);
    renderer.vertex_buffer = buffers[0];
    renderer.element_buffer = buffers[1];

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &renderer.vertex_array);

    glBindVertexArray(renderer.vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, position)));
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, uv)));
    glEnableVertexAttribArray(1);

    // normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, normal)));
    glEnableVertexAttribArray(2);

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, color)));
    glEnableVertexAttribArray(3);

    return renderer;
}

void sprite_renderer_destroy(SpriteRenderer *renderer) {
    u32 buffers[] = {renderer->vertex_buffer, renderer->element_buffer};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &renderer->vertex_array);

    program_destory(&renderer->program);
}

void sprite_renderer_begin(SpriteRenderer *renderer) {
    glUseProgram(renderer->program.handle);
    glBindVertexArray(renderer->vertex_array);
}

void sprite_renderer_draw(SpriteRenderer *renderer) {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void sprite_renderer_end(SpriteRenderer *renderer) {
    glBindVertexArray(0);
}
