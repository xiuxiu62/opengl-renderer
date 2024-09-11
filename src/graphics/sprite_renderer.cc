#include "core/logger.h"
#include "core/types.h"
#include "graphics/program.h"
#include "graphics/sprite_renderer.h"
#include "graphics/texture.h"
#include "math/rotor.h"
#include "resources/image.h"
#include "sprite.h"

static Texture texture;
static Sprite sprite{
    .width = 1,
    .height = 1,
    .transform =
        {
            .position = {1, 0}, .scale = Vec2::ONE(), .rotation = Rot2::IDENTITY(),
            // .rotation = Rot2::from_angle(50),
        },
};

SpriteRenderer sprite_renderer_create(Camera *camera, PointLight light) {
    SpriteRenderer renderer{
        .camera = camera,
        .light = light,
    };

    Sprite::Vertex vertices[4];
    sprite_calculate_vertices(&sprite, vertices);

    static constexpr usize shader_count = 2;
    ShaderSource shader_sources[shader_count];
    shader_sources[0] = shader_source_load("assets/shaders/sprite.vert", VERT);
    shader_sources[1] = shader_source_load("assets/shaders/sprite.frag", FRAG);
    renderer.program = program_create(shader_sources, shader_count);

    texture = texture_create(image_load("assets/textures/brick.jpg"));

    u32 buffers[2];
    glGenBuffers(2, buffers);
    renderer.vertex_buffer = buffers[0];
    renderer.element_buffer = buffers[1];

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &renderer.vertex_array);

    glBindVertexArray(renderer.vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, Sprite::VERTEX_STORAGE_SIZE, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sprite::INDEX_STORAGE_SIZE, Sprite::INDICES, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, position)));
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, uv)));
    glEnableVertexAttribArray(1);

    // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, normal)));
    glEnableVertexAttribArray(2);

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex),
                          reinterpret_cast<void *>(offsetof(Sprite::Vertex, color)));
    glEnableVertexAttribArray(3);

    return renderer;
}

void sprite_renderer_destroy(SpriteRenderer *renderer) {
    texture_destroy(texture);

    u32 buffers[] = {renderer->vertex_buffer, renderer->element_buffer};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &renderer->vertex_array);

    program_destory(&renderer->program);
}

void sprite_renderer_begin(SpriteRenderer *renderer) {
    program_use(&renderer->program);

    // texutre uniform
    glUniform1i(glGetUniformLocation(renderer->program.handle, "texture_sampler"), 0);
    texture_use(texture);

    glBindVertexArray(renderer->vertex_array);

    u32 camera_location = glGetUniformLocation(renderer->program.handle, "camera");
    glUniformMatrix4fv(camera_location, 1, GL_FALSE, reinterpret_cast<f32 *>(&renderer->camera->combined_matrix));

    Mat4 transform_matrix = sprite.transform.to_mat4();
    u32 tranform_location = glGetUniformLocation(renderer->program.handle, "transform");
    glUniformMatrix4fv(tranform_location, 1, GL_FALSE, reinterpret_cast<f32 *>(&transform_matrix));

    u32 light_position_location = glGetUniformLocation(renderer->program.handle, "light.position");
    u32 light_color_location = glGetUniformLocation(renderer->program.handle, "light.color");
    u32 light_intensity_location = glGetUniformLocation(renderer->program.handle, "light.intensity");
    u32 light_radius_location = glGetUniformLocation(renderer->program.handle, "light.radius");

    glUniform3fv(light_position_location, 1, reinterpret_cast<f32 *>(&renderer->light.position));
    glUniform3fv(light_color_location, 1, reinterpret_cast<f32 *>(&renderer->light.color));
    glUniform1f(light_intensity_location, renderer->light.intensity);
    glUniform1f(light_radius_location, renderer->light.radius);
}

void sprite_renderer_draw(SpriteRenderer *renderer) {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void sprite_renderer_end(SpriteRenderer *renderer) {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
