#include "window.h"

#include <stdexcept>

Window::Window(int width, int height, const char *title) : _handle(nullptr) {
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("GLFW init failed");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    _handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_handle) {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
    }

    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);
}

Window::~Window() {
    if (_handle) {
        glfwDestroyWindow(_handle);
    }
    glfwTerminate();
}

bool Window::should_close() const {
    return glfwWindowShouldClose(_handle) == GLFW_TRUE;
}

void Window::swap_buffers() {
    glfwSwapBuffers(_handle);
}

void Window::poll_events() {
    glfwPollEvents();
}

GLFWwindow *Window::handle() const {
    return _handle;
}

void Window::get_framebuffer_size(int &width, int &height) const {
    glfwGetFramebufferSize(_handle, &width, &height);
}
