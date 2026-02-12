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

inline Engine::Engine()
    : _window(1280, 720, "Turtle-OS"), _renderer(_window) {
    _renderer.init();

    _canvases[0].set_color(0.15f, 0.18f, 0.2f, 1.0f);

    _canvases[1].set_texture_path(
        "data/biology/animals/birds/blue_tit/images/323241-2390601728.jpg");
    //_canvases[1].set_color(0.2f, 0.22f, 0.24f, 1.0f);

    _canvases[2].set_texture_path(
        "data/biology/animals/mammals/fox/images/fox-in-snow.jpg");
    //_canvases[2].set_color(0.18f, 0.2f, 0.22f, 1.0f);

    _canvases[3].set_color(0.1f, 0.12f, 0.14f, 1.0f);

    _canvases[4].set_folder_path("data");
    //_canvases[4].set_color(0.2f, 0.2f, 0.2f, 1.0f);
}

#endif // TURTLE_OS_ENGINE_H
