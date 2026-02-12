#include "engine.h"

#include <array>

namespace {
struct Viewport {
    int x;
    int y;
    int width;
    int height;
};

std::array<Viewport, 4> build_grid_viewports(int width, int height, float scale) {
    const int grid_width = static_cast<int>(width * scale);
    const int grid_height = static_cast<int>(height * scale);
    const int grid_x = (width - grid_width) / 2;
    const int grid_y = (height - grid_height) / 2;
    const int cell_width = grid_width / 2;
    const int cell_height = grid_height / 2;

    return {
        Viewport{grid_x, grid_y + cell_height, cell_width, cell_height},
        Viewport{grid_x + cell_width, grid_y + cell_height, cell_width, cell_height},
        Viewport{grid_x, grid_y, cell_width, cell_height},
        Viewport{grid_x + cell_width, grid_y, cell_width, cell_height}
    };
}

bool point_in_viewport(double x, double y, const Viewport &viewport) {
    return x >= viewport.x && x <= viewport.x + viewport.width &&
           y >= viewport.y && y <= viewport.y + viewport.height;
}
} // namespace

void Engine::run() {
    while (!_window.should_close()) {
        if (glfwGetKey(_window.handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            _window.request_close();
        }

        const bool left_down = glfwGetKey(_window.handle(), GLFW_KEY_LEFT) == GLFW_PRESS;
        const bool right_down = glfwGetKey(_window.handle(), GLFW_KEY_RIGHT) == GLFW_PRESS;
        if (left_down && !_left_was_down) {
            for (auto &canvas : _canvases) {
                if (canvas.source() == CanvasSource::Folder) {
                    canvas.prev();
                }
            }
        }
        if (right_down && !_right_was_down) {
            for (auto &canvas : _canvases) {
                if (canvas.source() == CanvasSource::Folder) {
                    canvas.next();
                }
            }
        }
        _left_was_down = left_down;
        _right_was_down = right_down;

        int width = 0;
        int height = 0;
        _window.get_framebuffer_size(width, height);

        int window_width = 0;
        int window_height = 0;
        glfwGetWindowSize(_window.handle(), &window_width, &window_height);
        if (window_width <= 0 || window_height <= 0) {
            window_width = width;
            window_height = height;
        }

        double cursor_x = 0.0;
        double cursor_y = 0.0;
        glfwGetCursorPos(_window.handle(), &cursor_x, &cursor_y);
        const double scale_x = static_cast<double>(width) /
                               static_cast<double>(window_width);
        const double scale_y = static_cast<double>(height) /
                               static_cast<double>(window_height);
        const double mouse_x = cursor_x * scale_x;
        const double mouse_y = (static_cast<double>(window_height) - cursor_y) * scale_y;

        const bool mouse_down =
            glfwGetMouseButton(_window.handle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        _renderer.begin_frame();

        _canvases[0].ensure_active_loaded();
        _renderer.render(_canvases[0], 0, 0, width, height);

        const auto viewports = build_grid_viewports(width, height, 0.5f);

        if (mouse_down) {
            if (!_dragging) {
                for (int i = 0; i < 4; ++i) {
                    Viewport viewport = viewports[static_cast<size_t>(i)];
                    const CanvasOffset &offset = _canvas_offsets[static_cast<size_t>(i + 1)];
                    viewport.x += offset.x;
                    viewport.y += offset.y;
                    if (point_in_viewport(mouse_x, mouse_y, viewport)) {
                        _dragging = true;
                        _drag_index = static_cast<size_t>(i + 1);
                        _drag_start_x = mouse_x;
                        _drag_start_y = mouse_y;
                        _drag_start_offset_x = offset.x;
                        _drag_start_offset_y = offset.y;
                        break;
                    }
                }
            } else if (_drag_index > 0 && _drag_index < _canvas_offsets.size()) {
                CanvasOffset &offset = _canvas_offsets[_drag_index];
                offset.x = _drag_start_offset_x +
                           static_cast<int>(mouse_x - _drag_start_x);
                offset.y = _drag_start_offset_y +
                           static_cast<int>(mouse_y - _drag_start_y);
            }
        } else {
            _dragging = false;
        }

        for (int i = 0; i < 4; ++i) {
            Canvas &canvas = _canvases[static_cast<size_t>(i + 1)];
            canvas.ensure_active_loaded();
            const CanvasOffset &offset = _canvas_offsets[static_cast<size_t>(i + 1)];
            _renderer.render(canvas,
                             viewports[static_cast<size_t>(i)].x + offset.x,
                             viewports[static_cast<size_t>(i)].y + offset.y,
                             viewports[static_cast<size_t>(i)].width,
                             viewports[static_cast<size_t>(i)].height);
        }

        _window.swap_buffers();
        _window.poll_events();
    }
}
