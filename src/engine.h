#ifndef TURTLE_OS_ENGINE_H
#define TURTLE_OS_ENGINE_H

#include "canvas.h"
#include "renderer.h"
#include "window.h"

#include <array>

class Engine {
public:
    Engine();
    void run();

private:
    Window _window;
    Renderer _renderer;
    std::array<Canvas, 5> _canvases;
};

#endif // TURTLE_OS_ENGINE_H
