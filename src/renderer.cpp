#include "renderer.h"

#include <GLES2/gl2.h>

Renderer::Renderer(Window &window)
    : _window(window), _shader(), _mesh(),
      _solid_texture_primary(0),
      _solid_color_primary{-1.0f, -1.0f, -1.0f, -1.0f},
      _solid_texture_secondary(0),
          _solid_color_secondary{-1.0f, -1.0f, -1.0f, -1.0f},
          _framebuffer(0) {}

Renderer::~Renderer() {
    if (_framebuffer != 0) {
        glDeleteFramebuffers(1, &_framebuffer);
    }
}

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
    bind_texture_handle(base_handle, 1);

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

void Renderer::render_texture(unsigned int texture, int x, int y, int width, int height) {
    glViewport(x, y, width, height);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1f("mix_factor", 0.0f);
    _shader.set_uniform1i("tex0", 0);
    _shader.set_uniform1i("tex1", 1);

    bind_texture_handle(texture, 0);
    bind_texture_handle(texture, 1);

    _mesh.draw(_shader);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render_canvas_to_texture(const Canvas &canvas,
                                        unsigned int output_texture,
                                        int width,
                                        int height) {
    begin_offscreen(output_texture, width, height);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1f("mix_factor", 0.0f);
    _shader.set_uniform1i("tex0", 0);
    _shader.set_uniform1i("tex1", 1);

    const unsigned int handle =
        bind_canvas_texture(canvas, 0, _solid_texture_primary, _solid_color_primary);
    bind_texture_handle(handle, 1);

    _mesh.draw(_shader);

    end_offscreen();
}

void Renderer::render_blend_texture_canvas(unsigned int base_texture,
                                           const Canvas &overlay,
                                           unsigned int output_texture,
                                           int width,
                                           int height,
                                           float mix_factor) {
    begin_offscreen(output_texture, width, height);

    _shader.use();
    _shader.set_uniform1f("scale", 1.0f);
    _shader.set_uniform1f("mix_factor", mix_factor);
    _shader.set_uniform1i("tex0", 0);
    _shader.set_uniform1i("tex1", 1);

    bind_texture_handle(base_texture, 0);
    bind_canvas_texture(overlay, 1, _solid_texture_secondary, _solid_color_secondary);

    _mesh.draw(_shader);

    end_offscreen();
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

void Renderer::bind_texture_handle(unsigned int texture, int texture_unit) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + texture_unit));
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Renderer::begin_offscreen(unsigned int output_texture, int width, int height) {
    if (_framebuffer == 0) {
        glGenFramebuffers(1, &_framebuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, output_texture, 0);
    glViewport(0, 0, width, height);
}

void Renderer::end_offscreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
