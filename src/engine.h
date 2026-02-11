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

   class OverlayCanvas : public Image::Texture
   {
   protected:
      OverlayCanvas(const std::string &path, float x, float y, float w, float h, float z)
         : Texture(path)
      {
         set_quad(x, y, w, h, z);
      }

      OverlayCanvas(const vec4 &color, float x, float y, float w, float h, float z)
         : Texture(color)
      {
         set_quad(x, y, w, h, z);
      }
   };

   class Canvas1 : public OverlayCanvas
   {
   public:
      Canvas1()
         : OverlayCanvas("data/biology/animals/mammals/camel/images/camel-1279353277.jpg",
                         -1.0f, 1.0f, 0.5f, 0.5f, 0.1f)
      {
      }
   };

   class Canvas2 : public OverlayCanvas
   {
   public:
      Canvas2()
         : OverlayCanvas("data/biology/animals/mammals/cat/images/cat.jpg",
                         0.0f, 1.0f, 0.5f, 0.5f, 0.1f)
      {
      }
   };

   class Canvas3 : public OverlayCanvas
   {
   public:
      Canvas3()
         : OverlayCanvas(vec4{0.2f, 0.6f, 0.8f, 1.0f}, -1.0f, 0.5f, 0.5f, 0.5f, 0.05f)
      {
      }
   };

   class Canvas4 : public OverlayCanvas
   {
   public:
      Canvas4()
         : OverlayCanvas(vec4{0.9f, 0.4f, 0.2f, 1.0f}, 0.0f, 0.5f, 0.5f, 0.5f, 0.05f)
      {
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
   void update_overlay_quad(int index, float cx, float cy, float cw, float ch);

   GLSL _backend;
   Window _win;
   MainCanvas _main;
   Canvas1 _canvas1;
   Canvas2 _canvas2;
   Canvas3 _canvas3;
   Canvas4 _canvas4;
};
