

#include "engine.h"
#include "backend_GLSL.h"
#include "shader.h"

#include <filesystem>
#include <iostream>
#include <string>

Engine::Engine(int argc, char *argv[])
   : TurtleBase(argc, argv)
   , _win("Turtle-OS")
{
   DEBUG_LEVEL = DebugLevel::DETAILED;
   _win.set_key_callback(key_call_back);
   create_shaders(_main._zoom._shader_location);
   _front_uniform._shader_location = _main._zoom._shader_location;
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
      engine->_main.select_texture(engine->_main._image_index + 1);
      engine->_main._zoom.reset_start_scale();
   } else if (key == GLFW_KEY_LEFT) {
      engine->_main.select_texture(engine->_main._image_index - 1);
      engine->_main._zoom.reset_start_scale();
   }

   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   }
}

void Engine::run()
{
   while (!_win.should_close()) {
      GLSL::render_loop(_win.get_handle(), _main, _main._zoom);
      _main._zoom.run();

      TurtleBase::Uniform::set_active(&_front_uniform);
      GLSL::render_loop(_win.get_handle(), _front);

      // Swap buffers and poll events
      _win.swap_buffers();
      _win.poll_events();
   }
};
