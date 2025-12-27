#pragma once

#include "turtle_baseclass.h"
#include "window.h"

class Engine : TurtleBase
{
private:
  class MainCanvas : public Image::Texture
  {
  public:
    MainCanvas() : Texture("data"), _zoom(15.0f, 0.1f, 2.0f) { _zoom.reset_start_scale(); };
    Effects::ScaleVertices _zoom;
  };

public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  Window _win;
  MainCanvas _main;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
  
  void render_loop_GLSL();
};
