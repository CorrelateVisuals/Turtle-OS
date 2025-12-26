#include <filesystem>
#include <iostream>
#include <string>

#include "engine.h"
#include "image_finder.h"
#include "resources.h"
#include "shader.h"

Engine::Engine(int argc, char *argv[])
    : TurtleBase(argc, argv), _win("Turtle-OS")
{
  ImageFinder finder;
  finder.set_root("data");
  //_comp._texture._images = finder.find_all_images();
  _comp._new_texture._images = finder.find_all_images();

  // finder.randomize_order(_comp._texture._images);
  finder.randomize_order(_comp._new_texture._images);

  //  _comp._new_texture.select_texture(_comp._new_texture._image_index);
  _comp._new_texture.select_texture(_comp._new_texture._image_index);
  _comp._zoom_effect.reset_start_scale();

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
    // engine->_comp._texture.select_texture(
    // engine->_comp._texture._image_index + 1);
    engine->_comp._new_texture.select_texture(
        engine->_comp._new_texture._image_index + 1);
    engine->_comp._zoom_effect.reset_start_scale();
  }
  else if (key == GLFW_KEY_LEFT)
  {
    engine->_comp._new_texture.select_texture(
        engine->_comp._new_texture._image_index - 1);

       
    //     engine->_comp._texture.select_texture(
    // engine->_comp._texture._image_index - 1);
    engine->_comp._zoom_effect.reset_start_scale();
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

void Engine::run()
{

  create_shaders(_comp._zoom_effect._scale_location);

  // load_texture("data/cat.jpg");
  glfwSetWindowUserPointer(_win.get_handle(), this);

  // Main render loop
  while (!_win.should_close())
  {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Update viewport each frame (important on Pinebook!)
    int w, h;
    glfwGetFramebufferSize(_win.get_handle(), &w, &h);
    glViewport(0, 0, w, h);

    glBindTexture(GL_TEXTURE_2D, _comp._new_texture._current_image);
    glUniform1f(_comp._zoom_effect._scale_location,
                _comp._zoom_effect._texture_start_scale);
    //                 glBindTexture(GL_TEXTURE_2D, _comp._texture._current_image);
    // glUniform1f(_comp._zoom_effect._scale_location,
    //             _comp._zoom_effect._texture_start_scale);

    _comp._zoom_effect.run();

    // Pass vertex positions to the shader
    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          _comp._new_texture._vertices.data());
    //     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
    // _comp._texture._vertices.data());

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          _comp._new_texture._vertices.data() + 2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, _comp._new_texture._indices.data());
    //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
    //                       _comp._texture._vertices.data() + 2);

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, _comp._texture._indices.data());

    // Swap buffers and poll events
    _win.swap_buffers();
    _win.poll_events();
  }
};
