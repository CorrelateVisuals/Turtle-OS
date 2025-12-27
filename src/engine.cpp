#include <filesystem>
#include <iostream>
#include <string>

#include "engine.h"
#include "resources.h"
#include "shader.h"

Engine::Engine(int argc, char *argv[])
    : TurtleBase1(argc, argv), _win("Turtle-OS") {
  _tex1._zoom.reset_start_scale();
  _win.set_key_callback(key_call_back);
}

void Engine::key_call_back(GLFWwindow *window, int key, int, int action, int) {
  if (action != GLFW_PRESS)
    return;

  // Retrieve Engine* pointer stored in GLFW window
  Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

  if (!engine)
    return;

  if (key == GLFW_KEY_RIGHT) {

    engine->_tex1.select_texture(engine->_tex1._image_index + 1);
    engine->_tex1._zoom.reset_start_scale();

  } else if (key == GLFW_KEY_LEFT) {
    engine->_tex1.select_texture(engine->_tex1._image_index - 1);
    engine->_tex1._zoom.reset_start_scale();
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Engine::run() {

  create_shaders(_tex1._zoom._scale_location);

  // load_texture("data/cat.jpg");
  glfwSetWindowUserPointer(_win.get_handle(), this);

  // Main render loop
  while (!_win.should_close()) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Update viewport each frame (important on Pinebook!)
    int w, h;
    glfwGetFramebufferSize(_win.get_handle(), &w, &h);
    glViewport(0, 0, w, h);

    glBindTexture(GL_TEXTURE_2D, _tex1._current_image);
    glUniform1f(_tex1._zoom._scale_location,
                _tex1._zoom._texture_start_scale);

    _tex1._zoom.run();

    // Pass vertex positions to the shader
    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          _tex1._vertices.data());

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          _tex1._vertices.data() + 2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   _tex1._indices.data());

    // Swap buffers and poll events
    _win.swap_buffers();
    _win.poll_events();
  }
};
