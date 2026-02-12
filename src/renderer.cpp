#include "renderer.h"

#include <GLES2/gl2.h>

Renderer::Renderer(Window &window)
    : _window(window), _shader(), _mesh(),
      _solid_texture_primary(0),
      _solid_color_primary{-1.0f, -1.0f, -1.0f, -1.0f},
      _solid_texture_secondary(0),
      _solid_color_secondary{-1.0f, -1.0f, -1.0f, -1.0f} {}

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
    _shader.set_uniform1f("mix_factor", 0.0f);
    _shader.set_uniform1i("tex0", 0);
    _shader.set_uniform1i("tex1", 1);

    const unsigned int base_handle =
        bind_canvas_texture(canvas, 0, _solid_texture_primary, _solid_color_primary);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, base_handle);

    _mesh.draw(_shader);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render_blend(const Canvas &base,
                            const Canvas &overlay,
                            int x,
                            int y,
                            int width,
                            int height,
                            float mix_factor) {
    glViewport(x, y, width, height);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1f("mix_factor", mix_factor);
    _shader.set_uniform1i("tex0", 0);
    _shader.set_uniform1i("tex1", 1);

    bind_canvas_texture(base, 0, _solid_texture_primary, _solid_color_primary);
    bind_canvas_texture(overlay, 1, _solid_texture_secondary, _solid_color_secondary);

    _mesh.draw(_shader);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Renderer::bind_canvas_texture(const Canvas &canvas,
                                           int texture_unit,
                                           unsigned int &solid_texture,
                                           float solid_color[4]) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + texture_unit));
    if (canvas.source() == CanvasSource::Color) {
        ensure_solid_texture(canvas.color(), solid_texture, solid_color);
        glBindTexture(GL_TEXTURE_2D, solid_texture);
        return solid_texture;
    }

    const BaseTexture *texture = canvas.active_texture();
    if (texture && texture->is_loaded()) {
        glBindTexture(GL_TEXTURE_2D, texture->handle());
        return texture->handle();
    }

    ensure_solid_texture(canvas.color(), solid_texture, solid_color);
    glBindTexture(GL_TEXTURE_2D, solid_texture);
    return solid_texture;
}

void Renderer::ensure_solid_texture(const float *color,
                                    unsigned int &texture,
                                    float stored_color[4]) {
    bool changed = false;
    for (int i = 0; i < 4; ++i) {
        if (stored_color[i] != color[i]) {
            changed = true;
            break;
        }
    }
    if (!changed && texture != 0) {
        return;
    }

    stored_color[0] = color[0];
    stored_color[1] = color[1];
    stored_color[2] = color[2];
    stored_color[3] = color[3];

    if (texture == 0) {
        glGenTextures(1, &texture);
    }

    unsigned char pixel[4] = {
        static_cast<unsigned char>(color[0] * 255.0f),
        static_cast<unsigned char>(color[1] * 255.0f),
        static_cast<unsigned char>(color[2] * 255.0f),
        static_cast<unsigned char>(color[3] * 255.0f)
    };

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}
