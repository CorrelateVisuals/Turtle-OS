#ifndef TURTLE_OS_ENGINE_H
#define TURTLE_OS_ENGINE_H

#include "canvas.h"
#include "renderer.h"
#include "window.h"

class Engine {
public:
    Engine();
    void run();

private:
    Window _window;
    Renderer _renderer;
    Canvas _canvas;
};

#endif // TURTLE_OS_ENGINE_H
