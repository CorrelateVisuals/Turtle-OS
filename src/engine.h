#pragma once

#include "debug.h"
#include "backend_GLSL.h"
#include "base_shader_access.h"
#include "base_image.h"
#include "window.h"

class Engine
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
      FrontCanvas(const std::string &path = "data/biology/animals/mammals/camel/images/camel-1279353277.jpg")
         : Texture(path)
      {
         // Upper left corner, fill out without borders
         set_quad(-1.0f, 1.0f, 0.5f, 0.5f, 0.1f);
      }
   };

   class FrontCanvas2 : public Image::Texture
   {
   public:
      FrontCanvas2(const std::string &path = "data/biology/animals/mammals/cat/images/cat.jpg")
         : Texture(path)
      {
         // Upper right corner, fill out without borders
         set_quad(0.0f, 1.0f, 0.5f, 0.5f, 0.1f);
      }
   };

   class LowerCanvas : public Image::Texture
   {
   public:
      LowerCanvas(const std::string &path = "data/biology/animals/mammals/camel/images/camel-1279353277.jpg")
         : Texture(path)
      {
         // Extend from remaining front-canvas edge to bottom screen
         set_quad(-1.0f, 0.5f, 0.5f, 1.5f, 0.05f);
      }
   };

public:
   Engine(int argc, char *argv[]);
   ~Engine() = default;

   void run();
   void handle_mouse_button(int button, int action, int mods);
   void handle_cursor_pos(double xpos, double ypos);
   void handle_key(int key, int scancode, int action, int mods);

private:
   GLSL _backend;
   Window _win;
   MainCanvas _main;
   FrontCanvas _front;
   FrontCanvas2 _front2;
   LowerCanvas _lower;
};
