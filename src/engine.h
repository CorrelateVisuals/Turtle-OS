#ifndef TURTLE_OS_ENGINE_H
#define TURTLE_OS_ENGINE_H

#include "canvas.h"
#include "renderer.h"
#include "window.h"

#include <array>
#include <cstddef>

class Engine {
public:
    Engine();
    void run();

private:
    struct CanvasOffset {
        int x;
        int y;
    };

    Window _window;
    Renderer _renderer;
    std::array<Canvas, 5> _canvases;
    std::array<CanvasOffset, 5> _canvas_offsets{};
    unsigned int _output_texture_a = 0;
    unsigned int _output_texture_b = 0;
    int _output_texture_width = 0;
    int _output_texture_height = 0;
    bool _output_use_a = true;
    bool _output_initialized = false;
    std::size_t _last_blend_source = static_cast<std::size_t>(-1);
    bool _dragging = false;
    std::size_t _drag_index = 0;
    double _drag_start_x = 0.0;
    double _drag_start_y = 0.0;
    int _drag_start_offset_x = 0;
    int _drag_start_offset_y = 0;
    bool _left_was_down = false;
    bool _right_was_down = false;
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

    _canvases[3].set_texture_path(
        "data/biology/animals/birds/blue_tit/images/Titmouse-bird-close-up_1920x1080-1838700548.jpg");

    _canvases[4].set_folder_path("data");
    //_canvases[4].set_color(0.2f, 0.2f, 0.2f, 1.0f);
}

#endif // TURTLE_OS_ENGINE_H
