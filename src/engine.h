#pragma once

#include "backend_GLSL.h"
#include "turtle_baseclass.h"
#include "window.h"

class Engine : TurtleBase
{
private:
   class MainCanvas : public Image::TextureArray
   {
   public:
      MainCanvas()
         : TextureArray("data")
         , _zoom(15.0f, 0.1f, 2.0f, 0.2f){};
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

   static void key_call_back(GLFWwindow *window, int key, int seconds, int action, int mods);
};
