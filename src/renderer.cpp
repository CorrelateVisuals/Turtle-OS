#include "renderer.h"

#include <GLES2/gl2.h>

Renderer::Renderer(Window &window)
    : _window(window), _shader(), _mesh(), _white_texture(0) {}

void Renderer::init() {
    _shader.load_from_files("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    _mesh.init();

    glGenTextures(1, &_white_texture);
    glBindTexture(GL_TEXTURE_2D, _white_texture);

    unsigned char pixel[4] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render() {
    int width = 0;
    int height = 0;
    _window.get_framebuffer_size(width, height);

    glViewport(0, 0, width, height);
    glClearColor(0.08f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1i("tex", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _white_texture);

    _mesh.draw(_shader);

    glBindTexture(GL_TEXTURE_2D, 0);
}
