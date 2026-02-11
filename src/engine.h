#pragma once

#include "implementation.h"
#include "scale_vertices.h"
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
   turtle::Implementation _implementation;
   turtle::DynamicImageStage *_gallery_stage = nullptr;

   static void key_call_back(GLFWwindow *window, int key, int seconds, int action, int mods);
};
