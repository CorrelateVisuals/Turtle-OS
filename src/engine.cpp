#include <filesystem>
#include <iostream>
#include <string>

#include "engine.h"
#include "shader.h"

Engine::Engine(int argc, char *argv[])
    : TurtleBase(argc, argv), _win("Turtle-OS")
{
  _win.set_key_callback(key_call_back);
}

void Engine::key_call_back(GLFWwindow *window, int key, int, int action, int)
{
  if (action != GLFW_PRESS)
    return;

  // Retrieve Engine* pointer stored in GLFW window
  Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

  if (!engine)
    return;

  if (key == GLFW_KEY_RIGHT)
  {

    engine->_main.select_texture(engine->_main._image_index + 1);
    engine->_main._zoom.reset_start_scale();
  }
  else if (key == GLFW_KEY_LEFT)
  {
    engine->_main.select_texture(engine->_main._image_index - 1);
    engine->_main._zoom.reset_start_scale();
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  return;
}

void Engine::render_loop_GLSL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // Update viewport each frame (important on Pinebook!)
  int w, h;
  glfwGetFramebufferSize(_win.get_handle(), &w, &h);
  glViewport(0, 0, w, h);

  glBindTexture(GL_TEXTURE_2D, _main._current_image);
  glUniform1f(_main._zoom._scale_location,
              _main._zoom._texture_start_scale);

  // Pass vertex positions to the shader
  glEnableVertexAttribArray(0); // pos
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        _main._vertices.data());

  glEnableVertexAttribArray(1); // uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        _main._vertices.data() + 2);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                 _main._indices.data());
  return;
}

void Engine::run()
{
  create_shaders(_main._zoom._scale_location);
  glfwSetWindowUserPointer(_win.get_handle(), this);

  // Main render loop
  while (!_win.should_close())
  {

    render_loop_GLSL();
    _main._zoom.run();

    // Swap buffers and poll events
    _win.swap_buffers();
    _win.poll_events();
  }
};
