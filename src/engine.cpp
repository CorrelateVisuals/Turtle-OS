#include "engine.h"

#include <array>

namespace {
constexpr size_t kBackgroundIndex = 0;
constexpr size_t kOutputIndex = 1;
constexpr float kGridScale = 0.5f;
constexpr size_t kInvalidCanvasIndex = static_cast<size_t>(-1);

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

Viewport offset_viewport(const Viewport &viewport, int dx, int dy) {
    return {viewport.x + dx, viewport.y + dy, viewport.width, viewport.height};
}

bool viewports_overlap(const Viewport &a, const Viewport &b) {
    return a.x < b.x + b.width && a.x + a.width > b.x &&
           a.y < b.y + b.height && a.y + a.height > b.y;
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

        _canvases[kBackgroundIndex].ensure_active_loaded();
        _renderer.render(_canvases[kBackgroundIndex], 0, 0, width, height);

        const auto base_viewports = build_grid_viewports(width, height, kGridScale);
        auto viewport_for = [&](size_t canvas_index) {
            const Viewport base = base_viewports[canvas_index - 1];
            const CanvasOffset &offset = _canvas_offsets[canvas_index];
            return offset_viewport(base, offset.x, offset.y);
        };

        if (mouse_down) {
            if (!_dragging) {
                for (int i = 0; i < 4; ++i) {
                    Viewport viewport = viewport_for(static_cast<size_t>(i + 1));
                    if (point_in_viewport(mouse_x, mouse_y, viewport)) {
                        _dragging = true;
                        _drag_index = static_cast<size_t>(i + 1);
                        _drag_start_x = mouse_x;
                        _drag_start_y = mouse_y;
                        _drag_start_offset_x = _canvas_offsets[_drag_index].x;
                        _drag_start_offset_y = _canvas_offsets[_drag_index].y;
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

        size_t blend_source_index = kInvalidCanvasIndex;
        if (_canvases.size() > kOutputIndex) {
            const Viewport output_viewport = viewport_for(kOutputIndex);
            for (size_t i = 1; i < _canvases.size(); ++i) {
                if (i == kOutputIndex) {
                    continue;
                }
                const Viewport other = viewport_for(i);
                if (viewports_overlap(output_viewport, other)) {
                    blend_source_index = i;
                    break;
                }
            }

            _canvases[kOutputIndex].ensure_active_loaded();
            const Viewport output_viewport_offset = output_viewport;
            if (blend_source_index != kInvalidCanvasIndex) {
                _canvases[blend_source_index].ensure_active_loaded();
                _renderer.render_blend(_canvases[kOutputIndex],
                                       _canvases[blend_source_index],
                                       output_viewport_offset.x,
                                       output_viewport_offset.y,
                                       output_viewport_offset.width,
                                       output_viewport_offset.height,
                                       0.5f);
            }
        }

        for (int i = 0; i < 4; ++i) {
            Canvas &canvas = _canvases[static_cast<size_t>(i + 1)];
            canvas.ensure_active_loaded();
            const Viewport viewport = viewport_for(static_cast<size_t>(i + 1));
            if (static_cast<size_t>(i + 1) == kOutputIndex) {
                if (blend_source_index == kInvalidCanvasIndex) {
                    _renderer.render(canvas,
                                     viewport.x,
                                     viewport.y,
                                     viewport.width,
                                     viewport.height);
                }
                continue;
            }

            _renderer.render(canvas,
                             viewport.x,
                             viewport.y,
                             viewport.width,
                             viewport.height);
        }

        _window.swap_buffers();
        _window.poll_events();
    }
}
