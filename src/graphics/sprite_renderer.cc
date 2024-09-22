#include "graphics/sprite_renderer.h"

#include "core/logger.h"
#include "core/types.h"
#include "graphics/program.h"
#include "graphics/texture.h"
#include "math/rotor.h"
#include "resources/image.h"
#include "sprite/sprite.h"

static SpriteRenderer renderer;

void sprite_renderer_init(void) {
    static constexpr usize shader_count = 2;
    ShaderSource shader_sources[shader_count];
    shader_sources[0] = shader_source_load("assets/shaders/sprite.vert", VERT);
    shader_sources[1] = shader_source_load("assets/shaders/sprite.frag", FRAG);
    renderer.program = program_create(shader_sources, shader_count);

    glGenVertexArrays(1, &renderer.vertex_array);
    glBindVertexArray(renderer.vertex_array);

    u32 buffers[2];
    glGenBuffers(2, buffers);
    renderer.vertex_buffer = buffers[0];
    renderer.element_buffer = buffers[1];

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, Sprite::VERTEX_STORAGE_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sprite::INDEX_STORAGE_SIZE, nullptr, GL_DYNAMIC_DRAW);

    /* Initialize attributes */
#define make_attr(binding, size, kind, field)                                                                          \
    glVertexAttribPointer(binding, size, kind, GL_FALSE, sizeof(Sprite::Vertex),                                       \
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, field)));                                  \
    glEnableVertexAttribArray(binding);

    make_attr(0, 2, GL_FLOAT, position);
    make_attr(1, 2, GL_FLOAT, uv);
    make_attr(2, 3, GL_FLOAT, normal);
    make_attr(3, 3, GL_FLOAT, color);

    glBindVertexArray(0);
}

void sprite_renderer_deinit(void) {
    u32 buffers[] = {renderer.vertex_buffer, renderer.element_buffer};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &renderer.vertex_array);

    program_destory(renderer.program);
}

void sprite_renderer_begin(Camera &camera, PointLight &light) {
    program_use(renderer.program);
    glBindVertexArray(renderer.vertex_array);

    program_set(renderer.program, "texture_sampler", 0);
    program_set(renderer.program, "camera", &camera.matrix);
    program_set(renderer.program, "light.position", light.position);
    program_set(renderer.program, "light.color", light.color);
    program_set(renderer.program, "light.intensity", light.intensity);
    program_set(renderer.program, "light.radius", light.radius);
}

void sprite_renderer_draw(Sprite &sprite) {
    static u32 tranform_location = glGetUniformLocation(renderer.program.handle, "transform");

    // texutre uniform
    glUniform1i(glGetUniformLocation(renderer.program.handle, "texture_sampler"), 0);
    texture_use(sprite.texture);

    Mat4 transform_matrix = sprite.transform.to_mat4();
    glUniformMatrix4fv(tranform_location, 1, GL_FALSE, reinterpret_cast<f32 *>(&transform_matrix));

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, Sprite::VERTEX_STORAGE_SIZE, sprite.vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sprite::INDEX_STORAGE_SIZE, Sprite::INDICES, GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// void sprite_renderer_draw(SpriteRenderer &renderer, AnimatedSprite &sprite) {
//     static u32 tranform_location = glGetUniformLocation(renderer.program.handle, "transform");

//     // texutre uniform
//     glUniform1i(glGetUniformLocation(renderer.program.handle, "texture_sampler"), 0);
//     texture_use(sprite.sprite.texture);

//     Mat4 transform_matrix = sprite.transform.to_mat4();
//     glUniformMatrix4fv(tranform_location, 1, GL_FALSE, reinterpret_cast<f32 *>(&transform_matrix));

//     Sprite::Vertex vertices[4];
//     animated_sprite_calculate_vertices(&sprite, vertices);

//     glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
//     glBufferData(GL_ARRAY_BUFFER, Sprite::VERTEX_STORAGE_SIZE, vertices, GL_DYNAMIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.element_buffer);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sprite::INDEX_STORAGE_SIZE, Sprite::INDICES, GL_DYNAMIC_DRAW);

//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// }

void sprite_renderer_end(void) {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
