#pragma once

// #include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
   struct Config
   {
      std::string title = "Turtle-OS";
      bool fullscreen = true;
      int width = 1280;
      int height = 720;
      bool capture_cursor = true;
      bool raw_mouse_input = true;
   };

   explicit Window(const Config &config);
   ~Window();

   bool should_close() const;
   void swap_buffers();
   void poll_events();
   void set_key_callback(GLFWkeyfun callback);
   void set_mouse_button_callback(GLFWmousebuttonfun callback);

   GLFWwindow *get_handle() const;

private:
   GLFWwindow *_handle = nullptr;
   Config _config;
};
