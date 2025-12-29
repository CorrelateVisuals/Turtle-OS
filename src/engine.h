#pragma once

#include "turtle_baseclass.h"
#include "window.h"
#include "backend_GLSL.h"

class Engine : TurtleBase
{
private:
  class MainCanvas : public Image::Texture
  {
  public:
    MainCanvas() : Texture("data"), _zoom(15.0f, 0.1f, 2.0f) {};
    ScaleVertices _zoom;
  };

public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  GLSL _backend;
  Window _win;
  MainCanvas _main;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);

  // void render_loop_GLSL();
};
