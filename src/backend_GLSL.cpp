#include "backend_GLSL.h"

void GLSL::render_loop(GLFWwindow *win) {
   int w = 0;
   int h = 0;
   glfwGetFramebufferSize(win, &w, &h);
   glViewport(0, 0, w, h);

   glClearColor(0.08f, 0.1f, 0.12f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

void GLSL::texture_to_gpu(GLuint &texture, unsigned char *pixels, int width,
                          int height) {
   glGenTextures(1, &texture);

   glBindTexture(GL_TEXTURE_2D, texture);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
