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
} // namespace

void Engine::run() {
    while (!_window.should_close()) {
        if (glfwGetKey(_window.handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            _window.request_close();
        }

        int width = 0;
        int height = 0;
        _window.get_framebuffer_size(width, height);

        _renderer.begin_frame();

        _canvases[0].ensure_active_loaded();
        _renderer.render(_canvases[0], 0, 0, width, height);

        const auto viewports = build_grid_viewports(width, height, 0.5f);

        for (int i = 0; i < 4; ++i) {
            Canvas &canvas = _canvases[static_cast<size_t>(i + 1)];
            canvas.ensure_active_loaded();
            _renderer.render(canvas,
                             viewports[static_cast<size_t>(i)].x,
                             viewports[static_cast<size_t>(i)].y,
                             viewports[static_cast<size_t>(i)].width,
                             viewports[static_cast<size_t>(i)].height);
        }

        _window.swap_buffers();
        _window.poll_events();
    }
}
