#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow* handle;
    int width;
    int height;
    const char* title;
} Window;

Window* window_create(int width, int height, const char* title);
void window_destroy(Window* win);
void window_swap_buffers(Window* win);
int window_should_close(Window* win);

#endif
