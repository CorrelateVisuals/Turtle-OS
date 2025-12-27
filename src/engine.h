#pragma once

#include <GLES2/gl2.h>
#include <array>
#include <vector>

#include "geometry.h"
#include "turtle_baseclass.h"
#include "window.h"

class Engine : TurtleBase1 {
public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  Window _win;
  class MainCanvas : public TB::Image::Texture {
  public:
    MainCanvas() : _zoom(15.0f, 0.1f, 2.0f) { _zoom.reset_start_scale(); };

    TB::Effects::ScaleVertices _zoom;

  } _tex1;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
};
