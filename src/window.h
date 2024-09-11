#pragma once

#include "types.h"

#include <GLFW/glfw3.h>

typedef GLFWwindow Window;

Window *window_create(const char *title, u32 width, u32 height, bool enable_vsync = true);
void window_destroy(Window *window);
