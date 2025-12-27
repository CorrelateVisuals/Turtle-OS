#include "window.h"
#include <iostream>

Window::Window(std::string_view title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return;
  }

  // ---- Request OpenGL ES 2.0 ----
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // ---- FULLSCREEN MODE ----
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  _handle = glfwCreateWindow(mode->width, mode->height, title.data(),
                             monitor, // FULLSCREEN
                             nullptr);

  if (!_handle) {
    std::cerr << "Failed to create fullscreen window\n";
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(_handle);

  // ---- CAPTURE INPUT ----
  glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
}

Window::~Window() {
  if (_handle) {
    glfwDestroyWindow(_handle);
    glfwTerminate();
  }
}

bool Window::should_close() const { return glfwWindowShouldClose(_handle); }

void Window::swap_buffers() { glfwSwapBuffers(_handle); }

void Window::poll_events() { glfwPollEvents(); }

GLFWwindow *Window::get_handle() const { return _handle; }

void Window::set_key_callback(GLFWkeyfun callback) {
  glfwSetKeyCallback(_handle, callback);
}
