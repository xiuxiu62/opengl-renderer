#include "camera.h"
#include "core/debug.h"
#include "core/logger.h"
#include "core/time/clock.h"
#include "core/types.h"
#include "graphics/sprite_renderer.h"
#include "graphics/texture.h"
#include "math/transform.h"
#include "resources/image.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

static const char *TITLE = "example";
static const u32 WIDTH = 1920, HEIGHT = 1080;

static Camera camera;

void startup(void);
void shutdown(void);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam);
void handle_input(Window *window, Sprite &character_sprite, f64 delta_t);

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

    startup();

    PointLight light = {
        .position = {0, 0, 1},
        .color = {1, 1, 1},
        .intensity = 1.5,
    };

    camera = camera_create({WIDTH, HEIGHT}, Vec2::ZERO(), 100);
    SpriteRenderer sprite_renderer = sprite_renderer_create(camera, light);

    // Texture character = texture_create(image_load("assets/sprites/wizard/Walk.png"));
    Texture character = texture_create(image_load("assets/sprites/example_character.jpg"));
    Texture example_texture = texture_create(image_load("assets/textures/brick.jpg"));

    // AnimatedSprite character_sprite = {};

    Sprite character_sprite = Sprite::create(1, 1, Transform::DEFAULT(), character);
#define make_sprite(x, y) Sprite::create(2, 2, Transform::from_translation({x, y}), example_texture)
    Sprite sprites[5]{
        make_sprite(0, 0), make_sprite(-2, 0), make_sprite(2, 0), make_sprite(0, 2), make_sprite(0, -2),
    };

    ;

    while (!glfwWindowShouldClose(window)) {
        // pre-update
        glfwPollEvents();
        clock_update(global_clock);

        // update
        handle_input(window, character_sprite, global_clock.delta_t);

        // post-update
        camera_update_matrices(&camera);

        // pre-render
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        sprite_renderer_begin(sprite_renderer);
        for (usize i = 0; i < 5; i++)
            sprite_renderer_draw(sprite_renderer, sprites[i]);
        sprite_renderer_draw(sprite_renderer, character_sprite);
        sprite_renderer_end(sprite_renderer);

        // post-render
        glfwSwapBuffers(window);
        // break;
    }

    shutdown();

    texture_destroy(example_texture);
    texture_destroy(character);

    sprite_renderer_destroy(sprite_renderer);
    window_destroy(window);

    return 0;
}

void startup() {
    image_manager_init();
    global_clock_init();
}

void shutdown() {
    image_manager_deinit();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera_update_viewport(&camera, {static_cast<u32>(width), static_cast<u32>(height)});
}

void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam) {
    error("[GL] %s", message);
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
    on_press(GLFW_KEY_Q, zoom_direction -= 1.0f);
    on_press(GLFW_KEY_E, zoom_direction += 1.0f);

    if (move_direction.x != 0 || move_direction.y != 0) {
        Vec2 move_amount = move_direction.normalized() * move_speed * delta_t;
        camera_move(&camera, move_amount);
        character_sprite.transform.translation += move_amount;
    }

    if (zoom_direction != 0) {
        f32 zoom_factor = 1.0f + (zoom_direction * zoom_speed * delta_t);
        camera_zoom(&camera, zoom_factor);
    }
}
