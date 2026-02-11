#pragma once

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>


class GLSL {
public:
   static void render_loop(GLFWwindow *win);
   static void texture_to_gpu(GLuint &texture, unsigned char *pixels, int width,
                              int height);
};
