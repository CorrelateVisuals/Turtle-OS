#include <iostream>

#include "window.h"

Window::Window(int width, int height, std::string_view title)
    : handle(nullptr)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Request an OpenGL ES 2.0 context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (!handle) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(handle);
}

Window::~Window()
{
    if (handle) {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle);
}

void Window::swapBuffers()
{
    if (handle)
        glfwSwapBuffers(handle);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

GLFWwindow* Window::getHandle() const
{
    return handle;
}

void Window::setKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(handle, callback);
}
