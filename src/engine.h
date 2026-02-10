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
      MainCanvas(std::string folder = "data")
         : TextureArray(folder)
         , _zoom(15.0f, 0.1f, 2.0f, 0.2f){};
      ScaleVertices _zoom;
   };

   class FrontCanvas : public Image::Texture
   {
   public:
      FrontCanvas(const std::string& path)
         : Texture(path) {
         // About 1/8th of the screen: sqrt(1/8) ≈ 0.35
         float w = 0.35f;
         float h = 0.35f;
         float x0 = -1.0f + 0.02f; // left edge, small inset
         float y0 = 1.0f - 0.02f;  // top edge, small inset
         float z  = 0.1f;
         _scale = w;
         _position = {x0, y0, z};
         _vertices = {
            x0,      y0,      z, 0.0f, 1.0f, // Top-left (u=0,v=1)
            x0,      y0-h,    z, 0.0f, 0.0f, // Bottom-left (u=0,v=0)
            x0+w,    y0-h,    z, 1.0f, 0.0f, // Bottom-right (u=1,v=0)
            x0+w,    y0,      z, 1.0f, 1.0f  // Top-right (u=1,v=1)
         };
      }
   };

public:
   Engine(int argc, char *argv[]);
   ~Engine() = default;

   void run();


private:
   GLSL _backend;
   Window _win;
   MainCanvas _main;
   FrontCanvas _front;
   TurtleBase::Uniform _front_uniform{0, 1.0f}; // static scale, no zoom

   static void key_call_back(GLFWwindow *window, int key, int seconds, int action, int mods);
};
