

#include "engine.h"
#include "backend_GLSL.h"
#include "shader.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace
{
constexpr char DEFAULT_IMAGE_PATH[] =
   "data/biology/animals/mammals/camel/images/camel-1279353277.jpg";
}

Engine::MainCanvasStage::MainCanvasStage(std::string folder)
   : _textures(std::move(folder))
   , _zoom(15.0f, 0.1f, 2.0f, 0.2f)
{
}

void Engine::MainCanvasStage::update()
{
   _zoom.run();
}

void Engine::MainCanvasStage::render(GLFWwindow *window)
{
   GLSL::render_loop(window, _textures, _zoom);
}

void Engine::MainCanvasStage::next_texture()
{
   _textures.select_texture(_textures.get_image_index() + 1);
   _zoom.reset_start_scale();
}

void Engine::MainCanvasStage::previous_texture()
{
   _textures.select_texture(_textures.get_image_index() - 1);
   _zoom.reset_start_scale();
}

turtle::ScaleVertices &Engine::MainCanvasStage::zoom()
{
   return _zoom;
}

Engine::StaticCanvasStage::StaticCanvasStage(const std::string &path, float x, float y,
                                             float width, float height, float depth)
   : _texture(path)
{
   _texture.set_quad(x, y, width, height, depth);
}

void Engine::StaticCanvasStage::update()
{
}

void Engine::StaticCanvasStage::render(GLFWwindow *window)
{
   GLSL::render_loop(window, _texture, &_texture.uniform());
}

turtle::Texture &Engine::StaticCanvasStage::texture()
{
   return _texture;
}

Engine::FrontCanvasStage::FrontCanvasStage()
   : StaticCanvasStage(DEFAULT_IMAGE_PATH, -1.0f, 1.0f, 0.5f, 0.5f, 0.1f)
{
}

Engine::LowerCanvasStage::LowerCanvasStage()
   : StaticCanvasStage(DEFAULT_IMAGE_PATH, -1.0f, 0.5f, 0.5f, 1.5f, 0.05f)
{
}

Engine::Engine(int argc, char *argv[])
   : TurtleBase(argc, argv)
   , _win(Window::Config{"Turtle-OS", true, 1280, 720, true, true})
   , _main_stage()
   , _front_stage()
   , _lower_stage()
{
   DEBUG_LEVEL = DebugLevel::DETAILED;
   _win.set_key_callback(key_call_back);
    
   create_shaders(_main_stage.zoom());
   _stage_manager.add_stage(_main_stage);
   _stage_manager.add_stage(_lower_stage);
   _stage_manager.add_stage(_front_stage);
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
      engine->_main_stage.next_texture();
   } else if (key == GLFW_KEY_LEFT) {
      engine->_main_stage.previous_texture();
   }

   if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, true);
   }
}

void Engine::run()
{
   while (!_win.should_close()) {
      _stage_manager.update();
      _stage_manager.render(_win.get_handle());

      // Swap buffers and poll events
      _win.swap_buffers();
      _win.poll_events();
   }
};
