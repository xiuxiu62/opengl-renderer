#include "camera.h"
#include "core/debug.h"
#include "core/file.h"
#include "core/logger.h"
#include "core/time/clock.h"
#include "core/types.h"
#include "graphics/sprite_renderer.h"
#include "graphics/text_renderer.h"
#include "graphics/texture.h"
#include "math/algorithm.h"
#include "math/transform.h"
#include "resources/image.h"
#include "systems/audio.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <glad/glad.h>

static const char *TITLE = "example";
static const u32 WIDTH = 1920, HEIGHT = 1080;

static Camera camera;
static f32 font_scale = 0.5f;

void startup(void);
void shutdown(void);

void handle_input(Window *window, Sprite &character_sprite, f64 delta_t);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam);

int main(void) {
    Window *window = window_create(TITLE, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        info("Failed to initialize GLAD");
        return -1;
    }

    when_debug({
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_message, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    });

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    startup();

    PointLight light = {
        .position = {0, 0, 1},
        .color = {1, 1, 1},
        .intensity = 1.5,
    };
    camera = camera_create({WIDTH, HEIGHT}, Vec2::ZERO(), 10.0f);

    Texture character = texture_create_pixel_art(image_load("assets/sprites/wizard/Still.png"));
    Texture example_texture = texture_create(image_load("assets/textures/brick.jpg"));

    Sprite character_sprite = Sprite::create(1, 1, Transform::from_scale({5, 5}), character);
#define make_sprite(x, y) Sprite::create(2, 2, Transform::from_translation({x, y}), example_texture)
    Sprite sprites[5]{
        make_sprite(0, 0), make_sprite(-2, 0), make_sprite(2, 0), make_sprite(0, 2), make_sprite(0, -2),
    };

    u32 example_song = audio_load("assets/music/the_veiled_monolith_proprietary.mp3");
    audio_play(example_song);

    while (!glfwWindowShouldClose(window)) {
        // pre-update
        glfwPollEvents();
        clock_update(global_clock);

        // update
        handle_input(window, character_sprite, global_clock.delta_t);

        static char position_buf[32];
        static char ortho_size_buf[32];
        sprintf(position_buf, "position: (%.2f, %.2f)", camera.position.x, camera.position.y);
        sprintf(ortho_size_buf, "zoom: %.2f", camera.ortho_size);

        // post-update
        camera_update_matrix(camera);

        // pre-render
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        sprite_renderer_begin(camera, light);
        for (usize i = 0; i < 5; i++)
            sprite_renderer_draw(sprites[i]);
        sprite_renderer_draw(character_sprite);
        sprite_renderer_end();

        text_renderer_begin();
        text_renderer_draw({20, 1000}, font_scale, position_buf);
        text_renderer_draw({20, 20}, font_scale, ortho_size_buf);
        text_renderer_end();

        // post-render
        glfwSwapBuffers(window);

        // break;
    }
    audio_remove(example_song);

    shutdown();

    texture_destroy(example_texture);
    texture_destroy(character);

    window_destroy(window);

    return 0;
}

void startup() {
    image_manager_init();
    audio_init();
    global_clock_init();
    sprite_renderer_init();
    text_renderer_init();
}

void shutdown() {
    text_renderer_deinit();
    sprite_renderer_deinit();
    audio_deinit();
    image_manager_deinit();
}

void handle_input(Window *window, Sprite &character_sprite, f64 delta_t) {
#define on_press(key, ...)                                                                                             \
    if (glfwGetKey(window, key) == GLFW_PRESS) __VA_ARGS__;
#define on_release(key, ...)                                                                                           \
    if (glfwGetKey(window, key) == GLFW_RELEASE) __VA_ARGS__;

    static constexpr f32 move_speed = 0.75f;
    static constexpr f32 zoom_speed = 1.25f;

    Vec2 move_direction = Vec2::ZERO();
    f32 zoom_direction = 0.0f;
    // f32 zoom_factor = 1.0f;

    on_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    on_press(GLFW_KEY_W, move_direction.y += 1.0f);
    on_press(GLFW_KEY_S, move_direction.y -= 1.0f);
    on_press(GLFW_KEY_A, move_direction.x -= 1.0f);
    on_press(GLFW_KEY_D, move_direction.x += 1.0f);
    on_press(GLFW_KEY_Q, zoom_direction += 1.0f);
    on_press(GLFW_KEY_E, zoom_direction -= 1.0f);

    const f32 FONT_SCALE_MIN = 0.25f, FONT_SCALE_MAX = 2.0f;
    on_press(GLFW_KEY_COMMA, font_scale = math::clamp(font_scale + 0.01, FONT_SCALE_MIN, FONT_SCALE_MAX));
    on_press(GLFW_KEY_PERIOD, font_scale = math::clamp(font_scale - 0.01, FONT_SCALE_MIN, FONT_SCALE_MAX));

    // TODO: fix coordinate system so i don't have to do all this corrective bullshit
    if (move_direction.x != 0 || move_direction.y != 0) {
        Vec2 move_amount = move_direction.normalized() * move_speed * Vec2{1.0, 1.25} * delta_t;

        character_sprite.transform.translation += move_amount * camera.ortho_size * Vec2{1.875f, -1.0};
        // info("(%f, %f)", move_amount.x, move_amount.y);
        // info("(%f, %f)", (move_amount * camera.zoom).x, (move_amount * camera.zoom).y);
        camera_move(camera, move_amount);
    }

    if (zoom_direction != 0) {
        f32 zoom_factor = 1.0f + (zoom_direction * zoom_speed * delta_t);
        camera_zoom(camera, zoom_factor);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera_update_viewport(camera, {static_cast<u32>(width), static_cast<u32>(height)});
}

void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam) {
    const char *sourceString;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        sourceString = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceString = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceString = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceString = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceString = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        sourceString = "Other";
        break;
    default:
        sourceString = "Unknown";
    }
    const char *typeString;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        typeString = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeString = "Deprecated Behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeString = "Undefined Behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeString = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeString = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        typeString = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        typeString = "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        typeString = "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeString = "Other";
        break;
    default:
        typeString = "Unknown";
    }
    printf("[GL %s:%s] %s (ID: %u, Severity: %d)\n\n", sourceString, typeString, message, id, severity);
}
