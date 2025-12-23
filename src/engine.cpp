#include <filesystem>
#include <iostream>
#include <string>

#include "engine.h"
#include "image_finder.h"
#include "resources.h"
#include "shader.h"
#include "geometry.h"

Engine::Engine(int argc, char* argv[]) : TurtleBase(argc, argv), _window("Turtle-OS") {
  ImageFinder finder;
  finder.set_root("data");
  _texture._images = finder.find_all_images();

  finder.randomize_order(_texture._images);

  _texture.select_texture(_texture._image_index, _zoom_effect);

  _window.set_key_callback(key_call_back);
}

void Engine::key_call_back(GLFWwindow *window, int key, int, int action, int) {
  if (action != GLFW_PRESS)
    return;

  // Retrieve Engine* pointer stored in GLFW window
  Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

  if (!engine)
    return;

  if (key == GLFW_KEY_RIGHT) {
    engine->_texture.select_texture(engine->_texture._image_index + 1, engine->_zoom_effect);
  } else if (key == GLFW_KEY_LEFT) {
    engine->_texture.select_texture(engine->_texture._image_index - 1, engine->_zoom_effect);
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Engine::run() {

  TurtleBase::Shaders::create_shaders(_zoom_effect._scale_location);
  
  // load_texture("data/cat.jpg");
  glfwSetWindowUserPointer(_window.get_handle(), this);

  // Main render loop
  while (!_window.should_close()) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Update viewport each frame (important on Pinebook!)
    int w, h;
    glfwGetFramebufferSize(_window.get_handle(), &w, &h);
    glViewport(0, 0, w, h);

    glBindTexture(GL_TEXTURE_2D, _texture._current_image);
    glUniform1f(_zoom_effect._scale_location,
                _zoom_effect._texture_start_scale);

    _zoom_effect.run();

    // Pass vertex positions to the shader
    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          RECT_VERTICES.data());

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          RECT_VERTICES.data() + 2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, INDICES.data());

    // Swap buffers and poll events
    _window.swap_buffers();
    _window.poll_events();
  }
};
