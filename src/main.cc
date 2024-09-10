#include "graphics/sprite_renderer.h"
#include "logger.h"
#include "types.h"
#include "utils.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

static const char *TITLE = "example";
static const u32 WIDTH = 1920, HEIGHT = 1080;

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

    SpriteRenderer sprite_renderer = sprite_renderer_create();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        handle_input(window);

        glClear(GL_COLOR_BUFFER_BIT);

        sprite_renderer_draw(&sprite_renderer);

        glfwSwapBuffers(window);
    }

    sprite_renderer_destroy(&sprite_renderer);

    window_destroy(window);
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
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

    on_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    on_press(GLFW_KEY_W, {});
    on_press(GLFW_KEY_S, {});
    on_press(GLFW_KEY_A, {});
    on_press(GLFW_KEY_D, {});
}
