#pragma once

#include "texture.h"
#include "uniform.h"

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>


class GLSL
{
public:
   static void render_loop(GLFWwindow *win, const turtle::TextureArray &textures,
                           const turtle::Uniform &uniform);
   static void render_loop(GLFWwindow *win, const turtle::Texture &texture,
                           const turtle::Uniform *uniform = nullptr);
   static void texture_to_gpu(GLuint &textures, unsigned char *pixels, const int width,
                              const int height);

private:
   class ShapeRenderer
   {
   public:
      void draw(const turtle::Texture &texture, const turtle::Uniform *uniform) const;
   };

   static ShapeRenderer _shape_renderer;
};
