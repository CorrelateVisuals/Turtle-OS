#pragma once

// #include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Window
{
public:
   Window(std::string_view title);
   ~Window();

   bool should_close() const;
   void swap_buffers();
   void poll_events();
   void set_key_callback(GLFWkeyfun callback);
   void set_mouse_button_callback(GLFWmousebuttonfun callback);
   void set_cursor_pos_callback(GLFWcursorposfun callback);

   GLFWwindow *get_handle() const;

   static void mouse_button_call_back(GLFWwindow *window, int button, int action, int mods);
   static void cursor_pos_call_back(GLFWwindow *window, double xpos, double ypos);

private:
   GLFWwindow *_handle = nullptr;
};
