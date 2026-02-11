

#include "engine.h"
#include "scene.h"
#include "shader.h"

#include <array>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
constexpr char DEFAULT_IMAGE_PATH[] =
   "data/biology/animals/mammals/camel/images/camel-1279353277.jpg";
}

Engine::Engine(int argc, char *argv[])
   : TurtleBase(argc, argv)
   , _win(Window::Config{"Turtle-OS", true, 1280, 720, true, true})
   , _implementation(turtle::Scene{
        turtle::Image::folder("data"),
        turtle::Image::file(DEFAULT_IMAGE_PATH, turtle::ImagePreset::bottom_left_fill()),
        turtle::Image::file(DEFAULT_IMAGE_PATH, turtle::ImagePreset::top_left_quarter()),
        turtle::Image::color(std::array<float, 4>{0.9f, 0.1f, 0.1f, 1.0f},
                             turtle::ImagePreset::half_size_center())})
{
   DEBUG_LEVEL = DebugLevel::DETAILED;
   _win.set_key_callback(key_call_back);
   
   _gallery_stage = _implementation.first_dynamic_stage();
   if (!_gallery_stage) {
      throw std::runtime_error("No dynamic image stage configured for gallery");
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
};
