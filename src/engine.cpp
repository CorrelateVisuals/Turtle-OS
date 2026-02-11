#include "engine.h"

Engine::Engine()
    : _window(1280, 720, "Turtle-OS"), _renderer(_window) {
    _renderer.init();
}

void Engine::run() {
    while (!_window.should_close()) {
        _renderer.render();
        _window.swap_buffers();
        _window.poll_events();
    }
}
