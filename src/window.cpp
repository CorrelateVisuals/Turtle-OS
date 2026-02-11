#include "window.h"

#include <iostream>

Window::Window(const Config &config)
   : _config(config)
{
   if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW\n";
      return;
   }

   // ---- Request OpenGL ES 2.0 ----
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

   GLFWmonitor *monitor = nullptr;
   int width = _config.width;
   int height = _config.height;

   if (_config.fullscreen) {
      monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      if (mode) {
         glfwWindowHint(GLFW_RED_BITS, mode->redBits);
         glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
         glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
         glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
         width = mode->width;
         height = mode->height;
      }
   }

   _handle = glfwCreateWindow(width, height, _config.title.c_str(), monitor, nullptr);

   if (!_handle) {
      std::cerr << "Failed to create fullscreen window\n";
      glfwTerminate();
      return;
   }

   glfwMakeContextCurrent(_handle);

   // ---- CAPTURE INPUT ----
   if (_config.capture_cursor) {
      glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      if (_config.raw_mouse_input && glfwRawMouseMotionSupported()) {
         glfwSetInputMode(_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
      }
   }
   else {
      glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }
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
   glfwSwapBuffers(_handle);
}

void Window::poll_events()
{
   glfwPollEvents();
}

GLFWwindow *Window::get_handle() const
{
   return _handle;
}

void Window::set_key_callback(GLFWkeyfun callback)
{
   glfwSetKeyCallback(_handle, callback);
}

void Window::set_mouse_button_callback(GLFWmousebuttonfun callback)
{
   glfwSetMouseButtonCallback(_handle, callback);
}
