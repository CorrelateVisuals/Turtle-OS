#include "engine.h"

Engine::Engine()
    : _window(1280, 720, "Turtle-OS"), _renderer(_window) {
    _renderer.init();
    _canvas.set_color(0.15f, 0.18f, 0.2f, 1.0f);
}

void Engine::run() {
    while (!_window.should_close()) {
        _canvas.ensure_active_loaded();
        _renderer.render(_canvas);
        _window.swap_buffers();
        _window.poll_events();
    }
}
