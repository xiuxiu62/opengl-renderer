#include "camera.h"
#include "graphics/sprite_renderer.h"
#include "logger.h"
#include "types.h"
#include "utils.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

static const char *TITLE = "example";
static const u32 WIDTH = 1920, HEIGHT = 1080;

static Camera camera;

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

    camera = camera_create({WIDTH, HEIGHT}, Vec2::ZERO(), 1);
    SpriteRenderer sprite_renderer = sprite_renderer_create(&camera);

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
        sprite_renderer_begin(&sprite_renderer);
        sprite_renderer_draw(&sprite_renderer);
        sprite_renderer_end(&sprite_renderer);

        // post-render
        glfwSwapBuffers(window);
    }

    sprite_renderer_destroy(&sprite_renderer);

    window_destroy(window);
    return 0;
}

// struct Renderer {
//     Window *window;
//     Camera camera;
//     SpriteRenderer sprite_renderer;
// };

// Renderer renderer_create();

// void renderer_destroy(Renderer *renderer);

// void renderer_pre_update(Renderer *renderer);

// void renderer_update(Renderer *renderer);

// void renderer_post_update(Renderer *renderer);

// void renderer_pre_render(Renderer *renderer);

// void renderer_render(Renderer *renderer);

// void renderer_post_render(Renderer *renderer);

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

    Vec2 move_amount = Vec2::ZERO();
    f32 zoom_factor = 1;

    on_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    on_press(GLFW_KEY_W, move_amount.y += 0.01);
    on_press(GLFW_KEY_S, move_amount.y -= 0.01);
    on_press(GLFW_KEY_A, move_amount.x -= 0.01);
    on_press(GLFW_KEY_D, move_amount.x += 0.01);
    on_press(GLFW_KEY_Q, zoom_factor /= 1.1);
    on_press(GLFW_KEY_E, zoom_factor *= 1.1);

    if (move_amount.x != 0 || move_amount.y != 0) camera_move(&camera, move_amount);
    if (zoom_factor != 1) camera_zoom(&camera, zoom_factor);
}
