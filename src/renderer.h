#ifndef TURTLE_OS_RENDERER_H
#define TURTLE_OS_RENDERER_H

#include "canvas.h"
#include "mesh.h"
#include "shader.h"
#include "window.h"

class Renderer {
public:
    explicit Renderer(Window &window);
    ~Renderer();

    void init();
    void begin_frame();
    void render(const Canvas &canvas, int x, int y, int width, int height);
    void render_blend(const Canvas &base,
                      const Canvas &overlay,
                      int x,
                      int y,
                      int width,
                      int height,
                      float mix_factor);
    void render_texture(unsigned int texture, int x, int y, int width, int height);
    void render_canvas_to_texture(const Canvas &canvas,
                                  unsigned int output_texture,
                                  int width,
                                  int height);
    void render_blend_texture_canvas(unsigned int base_texture,
                                     const Canvas &overlay,
                                     unsigned int output_texture,
                                     int width,
                                     int height,
                                     float mix_factor);

private:
    Window &_window;
    Shader _shader;
    Mesh _mesh;
    unsigned int _solid_texture_primary;
    float _solid_color_primary[4];
    unsigned int _solid_texture_secondary;
    float _solid_color_secondary[4];
    unsigned int _framebuffer;

    void ensure_solid_texture(const float *color,
                              unsigned int &texture,
                              float stored_color[4]);
    unsigned int bind_canvas_texture(const Canvas &canvas,
                                     int texture_unit,
                                     unsigned int &solid_texture,
                                     float solid_color[4]);
    void bind_texture_handle(unsigned int texture, int texture_unit);
    void begin_offscreen(unsigned int output_texture, int width, int height);
    void end_offscreen();
};

#endif // TURTLE_OS_RENDERER_H
