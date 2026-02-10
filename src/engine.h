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
         _scale = 0.4f;  // Smaller than main
         _position = {-0.3f, 0.3f, 0.1f};  // Top-left, slightly forward
         float w = _scale;
         float h = _scale;
         float x0 = _position[0];
         float y0 = _position[1];
         float z  = _position[2];
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
