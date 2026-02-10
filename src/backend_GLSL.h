#pragma once

#include "turtle_baseclass.h"

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>


class GLSL
{
public:
   static void render_loop(GLFWwindow *win, const TurtleBase::Image::TextureArray &textures,
                           const TurtleBase::Uniform &uniform);
   static void render_loop(GLFWwindow *win, const TurtleBase::Image::Texture &texture);
   static void texture_to_gpu(GLuint &textures, unsigned char *pixels, const int width,
                              const int height);
};
