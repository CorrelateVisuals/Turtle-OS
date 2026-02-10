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
         _scale = 0.3f;           // smaller than main canvas
         _vertices = {            // Z = 0.1f positions it in front
            -0.5f, 0.5f,  0.0f, 1.0f,  // Top-left
            -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom-left
            0.5f,  -0.5f, 1.0f, 0.0f,  // Bottom-right
            0.5f,  0.5f,  1.0f, 1.0f   // Top-right
         };
      };
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

   static void key_call_back(GLFWwindow *window, int key, int seconds, int action, int mods);
};
