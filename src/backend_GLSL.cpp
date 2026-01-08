#include "backend_GLSL.h"

void GLSL::render_loop(GLFWwindow *win, TurtleBase::Image::TextureArray &textures, TurtleBase::Uniform &uniform)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update viewport each frame (important on Pinebook!)
    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    glViewport(0, 0, w, h);

    glBindTexture(GL_TEXTURE_2D, textures._current_image);

    // Send uniform
    glUniform1f(uniform._shader_location,
                uniform.get_uniform_value());

    // Pass vertex positions to the shader
    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          textures._vertices.data());

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                          textures._vertices.data() + 2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   textures._indices.data());
    return;
}

void GLSL::texture_to_gpu(GLuint &tex, unsigned char *pixels, const int width, const int height)
{
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return;
}
