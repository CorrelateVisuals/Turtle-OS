#pragma once

#include "debug.h"
#include "backend_GLSL.h"
#include "base_shader_access.h"
#include "base_image.h"
#include "window.h"

class Engine
{
private:
   class Step1 : public Image::Texture
   {
   public:
      Step1(Color color = {1.0f, 0.0f, 0.0f, 1.0f})
         : Texture(color)
      {
         // Upper left
         set_quad(-0.6f, 0.6f, 0.4f, 0.4f, 0.1f);
      }
   };

   class Step2 : public Image::Texture
   {
   public:
      Step2(Color color = {0.0f, 1.0f, 0.0f, 1.0f})
         : Texture(color)
      {
         // Upper right
         set_quad(0.2f, 0.6f, 0.4f, 0.4f, 0.1f);
      }
   };

   class Step3 : public Image::Texture
   {
   public:
      Step3(Color color = {0.0f, 0.0f, 1.0f, 1.0f})
         : Texture(color)
      {
         // Bottom left
         set_quad(-0.6f, -0.2f, 0.4f, 0.4f, 0.1f);
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
   ScaleVertices _dummy;
   Step1 _step1;
   Step2 _step2;
   Step3 _step3;
};
