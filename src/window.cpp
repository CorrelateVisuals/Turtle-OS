#include "window.h"
#include <stdio.h>
#include <stdlib.h>

Window* window_create(int width, int height, const char* title) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    Window* win = new Window;
    win->width = width;
    win->height = height;
    win->title = title;
    win->handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!win->handle) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        delete win;
        return NULL;
    }

    glfwMakeContextCurrent(win->handle);
    return win;
}

void window_destroy(Window* win) {
    if (!win) return;
    glfwDestroyWindow(win->handle);
    glfwTerminate();
    free(win);
}

void window_swap_buffers(Window* win) {
    glfwSwapBuffers(win->handle);
}

int window_should_close(Window* win) {
    return glfwWindowShouldClose(win->handle);
}
