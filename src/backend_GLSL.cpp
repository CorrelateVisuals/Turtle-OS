#include "backend_GLSL.h"


void GLSL::render_loop(GLFWwindow *win, const Image::TextureArray &textures,
                       const Uniform &uniform)
{
   glClearColor(0.1f, 0.1f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   // Update viewport each frame (important on Pinebook!)
   int w{0};
   int h{0};
   glfwGetFramebufferSize(win, &w, &h);
   glViewport(0, 0, w, h);

   draw_shape(textures, &uniform);
}

void GLSL::render_loop(GLFWwindow *win, const Image::Texture &texture,
                       const Uniform *uniform)
{
   (void)win;
   draw_shape(texture, uniform);
}

void GLSL::draw_shape(const Image::Texture &texture,
                      const Uniform *uniform)
{
   if (texture.get_image() == 0) return; // Skip if texture not loaded

   glBindTexture(GL_TEXTURE_2D, texture.get_image());
   if (uniform) {
      glUniform1f(uniform->get_shader_location(), uniform->get_uniform_value());
   }
   glEnableVertexAttribArray(0); // pos
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                         texture.get_vertices().data());
   glEnableVertexAttribArray(1); // uv
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                         texture.get_vertices().data() + 3);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, texture.get_indices().data());
}

void GLSL::texture_to_gpu(GLuint &texture, unsigned char *pixels, const int width,
                          const int height)
{
   glGenTextures(1, &texture);

   glBindTexture(GL_TEXTURE_2D, texture);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
