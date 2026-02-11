#ifndef TURTLE_OS_RENDERER_H
#define TURTLE_OS_RENDERER_H

#include "mesh.h"
#include "shader.h"
#include "window.h"

class Renderer {
public:
    explicit Renderer(Window &window);

    void init();
    void render();

private:
    Window &_window;
    Shader _shader;
    Mesh _mesh;
    unsigned int _white_texture;
};

#endif // TURTLE_OS_RENDERER_H
