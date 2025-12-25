#pragma once

#include <GLES2/gl2.h>
#include <array>
#include <vector>

#include "turtle_baseclass.h"
#include "window.h"

class Engine : public TurtleBase {
public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  class Composition : public TurtleBase::Composition {
  public:
    TurtleBase::Image::Zoom _zoom_effect{15.0f, 0.1f, 2.0f};
    TurtleBase::Image::Texture _texture{};
  };

private:
  Window _window;
  Composition _composition;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
};