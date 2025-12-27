#pragma once

#include <GLES2/gl2.h>
#include <array>
#include <vector>

#include "geometry.h"

#include "turtle_baseclass.h"

#include "window.h"

class Engine : public TurtleBase {
public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  Window _win;
  Img::Texture _tex1;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
};
