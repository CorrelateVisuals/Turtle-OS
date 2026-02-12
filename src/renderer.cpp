#include "renderer.h"

#include <GLES2/gl2.h>

Renderer::Renderer(Window &window)
        : _window(window), _shader(), _mesh(), _solid_texture(0),
            _solid_color{-1.0f, -1.0f, -1.0f, -1.0f} {}

void Renderer::init() {
    _shader.load_from_files("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    _mesh.init();
}

void Renderer::begin_frame() {
    glClearColor(0.08f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render(const Canvas &canvas, int x, int y, int width, int height) {
    glViewport(x, y, width, height);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1i("tex", 0);

    glActiveTexture(GL_TEXTURE0);
    if (canvas.source() == CanvasSource::Color) {
        ensure_solid_texture(canvas.color());
        glBindTexture(GL_TEXTURE_2D, _solid_texture);
    } else {
        const BaseTexture *texture = canvas.active_texture();
        if (texture && texture->is_loaded()) {
            glBindTexture(GL_TEXTURE_2D, texture->handle());
        } else {
            ensure_solid_texture(canvas.color());
            glBindTexture(GL_TEXTURE_2D, _solid_texture);
        }
    }

    _mesh.draw(_shader);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ensure_solid_texture(const float *color) {
    bool changed = false;
    for (int i = 0; i < 4; ++i) {
        if (_solid_color[i] != color[i]) {
            changed = true;
            break;
        }
    }
    if (!changed && _solid_texture != 0) {
        return;
    }

    _solid_color[0] = color[0];
    _solid_color[1] = color[1];
    _solid_color[2] = color[2];
    _solid_color[3] = color[3];

    if (_solid_texture == 0) {
        glGenTextures(1, &_solid_texture);
    }

    unsigned char pixel[4] = {
        static_cast<unsigned char>(color[0] * 255.0f),
        static_cast<unsigned char>(color[1] * 255.0f),
        static_cast<unsigned char>(color[2] * 255.0f),
        static_cast<unsigned char>(color[3] * 255.0f)
    };

    glBindTexture(GL_TEXTURE_2D, _solid_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}
