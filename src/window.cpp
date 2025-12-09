#include <iostream>

#include "window.h"

Window::Window(int width, int height, std::string_view title)
    : _handle(nullptr)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Request an OpenGL ES 2.0 context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    _handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    
    if (!_handle) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_handle);
}

Window::~Window()
{
    if (_handle) {
        glfwDestroyWindow(_handle);
        glfwTerminate();
    }
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(_handle);
}

void Window::swap_buffers()
{
    if (_handle)
        glfwSwapBuffers(_handle);
}

void Window::poll_events()
{
    glfwPollEvents();
}

GLFWwindow* Window::get_handle() const
{
    return _handle;
}

void Window::set_key_callback(GLFWkeyfun callback) {
    glfwSetKeyCallback(_handle, callback);
}
