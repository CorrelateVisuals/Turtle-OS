#ifndef TURTLE_OS_RENDERER_H
#define TURTLE_OS_RENDERER_H

#include "canvas.h"
#include "mesh.h"
#include "shader.h"
#include "window.h"

class Renderer {
public:
    explicit Renderer(Window &window);

    void init();
    void render(const Canvas &canvas);

private:
    Window &_window;
    Shader _shader;
    Mesh _mesh;
    unsigned int _solid_texture;
    float _solid_color[4];

    void ensure_solid_texture(const float *color);
};

#endif // TURTLE_OS_RENDERER_H
