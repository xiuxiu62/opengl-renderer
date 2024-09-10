#include "logger.h"
#include "window.h"

#include <GLFW/glfw3.h>

Window *window_create(const char *title, u32 width, u32 height) {
    if (!glfwInit()) {
        error("Failed to initialize glfw");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) error("Failed to create window");
    glfwMakeContextCurrent(window);
    return window;
}

void window_destroy(Window *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
