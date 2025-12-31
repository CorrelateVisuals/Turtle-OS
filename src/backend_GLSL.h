#pragma once

#include <GLFW/glfw3.h>
#include <GLES2/gl2.h>

#include "turtle_baseclass.h"

class GLSL
{
public:
    void static render_loop(GLFWwindow *win, TurtleBase::Image::TextureArray &tex, TurtleBase::ScaleVertices &zoom);
    void static texture_to_gpu(GLuint &tex, unsigned char *pixels, const int width,
                               const int height);

private:
};
