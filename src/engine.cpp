

#include "engine.h"
#include "scene.h"
#include "shader.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
constexpr char DEFAULT_IMAGE_PATH[] =
   "data/biology/animals/mammals/camel/images/camel-1279353277.jpg";
constexpr std::array<float, 4> RED_COLOR{0.9f, 0.1f, 0.1f, 1.0f};
constexpr std::array<float, 4> BLUE_COLOR{0.1f, 0.2f, 0.9f, 1.0f};
}

Engine::Engine(int argc, char *argv[])
   : TurtleBase(argc, argv)
   , _win(Window::Config{"Turtle-OS", true, 1280, 720, false, false})
   , _implementation(turtle::GalleryScene::compose(
        "data", DEFAULT_IMAGE_PATH, DEFAULT_IMAGE_PATH, RED_COLOR))
{
   DEBUG_LEVEL = DebugLevel::DETAILED;
   _win.set_key_callback(key_call_back);
   _win.set_mouse_button_callback(mouse_button_callback);
   
   _gallery_stage = _implementation.first_dynamic_stage();
   if (!_gallery_stage) {
      throw std::runtime_error("No dynamic image stage configured for gallery");
   }

   _color_stage = _implementation.first_color_stage();
   if (_color_stage) {
      const auto &current = _color_stage->color();
      _color_is_blue = std::equal(current.begin(), current.end(), BLUE_COLOR.begin());
   }

   create_shaders(_gallery_stage->zoom());
   glfwSetWindowUserPointer(_win.get_handle(), this);
}

void Engine::key_call_back(GLFWwindow *window, int key, int, int action, int)
{
   if (action != GLFW_PRESS)
      return;

   // Retrieve Engine* pointer stored in GLFW window
   Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

   if (!engine)
      return;

   if (key == GLFW_KEY_RIGHT) {
      if (engine->_gallery_stage)
         engine->_gallery_stage->next_texture();
   } else if (key == GLFW_KEY_LEFT) {
      if (engine->_gallery_stage)
         engine->_gallery_stage->previous_texture();
   }

   if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, true);
   }
}

void Engine::run()
{
   while (!_win.should_close()) {
      _implementation.update();
      _implementation.render(_win.get_handle());

      // Swap buffers and poll events
      _win.swap_buffers();
      _win.poll_events();
   }
}

void Engine::mouse_button_callback(GLFWwindow *window, int button, int action, int)
{
   if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
      return;

   Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
   if (!engine || !engine->_color_stage)
      return;

   double cursor_x = 0.0;
   double cursor_y = 0.0;
   glfwGetCursorPos(window, &cursor_x, &cursor_y);

   int width = 0;
   int height = 0;
   glfwGetWindowSize(window, &width, &height);
   if (width == 0 || height == 0)
      return;

   const float ndc_x = static_cast<float>((cursor_x / static_cast<double>(width)) * 2.0 - 1.0);
   const float ndc_y = static_cast<float>(1.0 - (cursor_y / static_cast<double>(height)) * 2.0);

   const auto &vertices = engine->_color_stage->get_vertices();
   const float left = std::min({vertices[0], vertices[5], vertices[10], vertices[15]});
   const float right = std::max({vertices[0], vertices[5], vertices[10], vertices[15]});
   const float top = std::max({vertices[1], vertices[6], vertices[11], vertices[16]});
   const float bottom = std::min({vertices[1], vertices[6], vertices[11], vertices[16]});

   if (ndc_x < left || ndc_x > right || ndc_y < bottom || ndc_y > top)
      return;

   if (engine->_color_is_blue) {
      engine->_color_stage->set_color(RED_COLOR);
      engine->_color_is_blue = false;
   }
   else {
      engine->_color_stage->set_color(BLUE_COLOR);
      engine->_color_is_blue = true;
   }
}
