#pragma once

#include <GLES2/gl2.h>
#include <vector>
#include <array>

#include "turtle_baseclass.h"
#include "window.h"

class Engine {
public:
  Engine();
  ~Engine() = default;

  void run();

private:
  Window _window;

  TurtleBase::Image::Zoom _zoom_effect{15.0f, 0.1f, 2.0f};

  GLuint _current_texture{0};
  std::vector<std::string> _textures;
  int _texture_index{0};

  void select_texture(int index);

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);

  void create_shaders();
};