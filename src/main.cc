#include "camera.h"
#include "core/debug.h"
#include "core/logger.h"
#include "core/types.h"
#include "graphics/sprite_renderer.h"
#include "graphics/texture.h"
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
void handle_input(Window *window);

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

    Texture texture = texture_create(image_load("assets/textures/brick.jpg"));
    constexpr usize sprite_count = 5;
    Sprite sprites[sprite_count]{

        {
            .width = 2,
            .height = 2,
            .transform =
                {
                    .position = {0, 0},
                    .scale = Vec2::ONE(),
                    .rotation = Rot2::IDENTITY(),
                },
            // .rotation = Rot2::from_angle(50),
            .texture = texture,
        },
        {
            .width = 2,
            .height = 2,
            .transform =
                {
                    .position = {-1, 0},
                    .scale = Vec2::ONE(),
                    .rotation = Rot2::IDENTITY(),
                },
            // .rotation = Rot2::from_angle(50),
            .texture = texture,
        },
        {
            .width = 2,
            .height = 2,
            .transform =
                {
                    .position = {1, 0},
                    .scale = Vec2::ONE(),
                    .rotation = Rot2::IDENTITY(),
                },
            // .rotation = Rot2::from_angle(50),
            .texture = texture,
        },
        {
            .width = 2,
            .height = 2,
            .transform =
                {
                    .position = {0, 1},
                    .scale = Vec2::ONE(),
                    .rotation = Rot2::IDENTITY(),
                },
            // .rotation = Rot2::from_angle(50),
            .texture = texture,
        },
        {
            .width = 2,
            .height = 2,
            .transform =
                {
                    .position = {0, -1},
                    .scale = Vec2::ONE(),
                    .rotation = Rot2::IDENTITY(),
                },
            // .rotation = Rot2::from_angle(50),
            .texture = texture,
        },
    };

    while (!glfwWindowShouldClose(window)) {
        // pre-update
        glfwPollEvents();
        // update
        handle_input(window);
        // post-update
        camera_update_matrices(&camera);

        // pre-render
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        sprite_renderer_begin(sprite_renderer);
        for (usize i = 0; i < sprite_count; i++) {
            sprite_renderer_draw(sprite_renderer, sprites[i]);
        }
        sprite_renderer_end(sprite_renderer);

        // post-render
        glfwSwapBuffers(window);
        // break;
    }

    shutdown();

    texture_destroy(texture);
    sprite_renderer_destroy(sprite_renderer);
    window_destroy(window);

    return 0;
}

void startup() {
    image_manager_init();
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

void handle_input(Window *window) {
#define on_press(key, ...)                                                                                             \
    if (glfwGetKey(window, key) == GLFW_PRESS) __VA_ARGS__;
#define on_release(key, ...)                                                                                           \
    if (glfwGetKey(window, key) == GLFW_RELEASE) __VA_ARGS__;

    static constexpr f32 move_speed = 0.01;
    static constexpr f32 zoom_speed = 1.05;

    Vec2 move_amount = Vec2::ZERO();
    f32 zoom_factor = 1;

    on_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    on_press(GLFW_KEY_W, move_amount.y += move_speed);
    on_press(GLFW_KEY_S, move_amount.y -= move_speed);
    on_press(GLFW_KEY_A, move_amount.x -= move_speed);
    on_press(GLFW_KEY_D, move_amount.x += move_speed);
    on_press(GLFW_KEY_Q, zoom_factor /= zoom_speed);
    on_press(GLFW_KEY_E, zoom_factor *= zoom_speed);

    if (move_amount.x != 0 || move_amount.y != 0) camera_move(&camera, move_amount);
    if (zoom_factor != 1) camera_zoom(&camera, zoom_factor);
}
