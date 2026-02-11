#ifndef TURTLE_OS_ENGINE_H
#define TURTLE_OS_ENGINE_H

#include "renderer.h"
#include "window.h"

class Engine {
public:
    Engine();
    void run();

private:
    Window _window;
    Renderer _renderer;
};

#endif // TURTLE_OS_ENGINE_H
