#include "logger.h"
#include "utils.h"
#include "window.h"

#include <GLFW/glfw3.h>

Window *window_create(const char *title, u32 width, u32 height, bool enable_vsync) {
    if (!glfwInit()) {
        error("Failed to initialize glfw");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    when_debug(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));

    Window *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) error("Failed to create window");
    glfwMakeContextCurrent(window);
    if (enable_vsync) glfwSwapInterval(1); // enable vsync
    return window;
}

void window_destroy(Window *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
