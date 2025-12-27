#pragma once

#include "turtle_baseclass.h"
#include "window.h"

class Engine : TurtleBase
{
public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  Window _win;

  class MainCanvas : public Image::Texture
  {
  public:
    MainCanvas() : _zoom(15.0f, 0.1f, 2.0f) { _zoom.reset_start_scale(); };

    Effects::ScaleVertices _zoom;
  } _tex1;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
};
