#include "engine.h"

Engine::Engine()
    : _window(1280, 720, "Turtle-OS"), _renderer(_window) {
    _renderer.init();
    _canvases[0].set_color(0.15f, 0.18f, 0.2f, 1.0f);
    for (size_t i = 1; i < _canvases.size(); ++i) {
        _canvases[i].set_folder_path("data");
    }
}

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

        const float grid_scale = 0.5f;
        const int grid_width = static_cast<int>(width * grid_scale);
        const int grid_height = static_cast<int>(height * grid_scale);
        const int grid_x = (width - grid_width) / 2;
        const int grid_y = (height - grid_height) / 2;
        const int cell_width = grid_width / 2;
        const int cell_height = grid_height / 2;

        const int viewports[4][4] = {
            {grid_x, grid_y + cell_height, cell_width, cell_height},
            {grid_x + cell_width, grid_y + cell_height, cell_width, cell_height},
            {grid_x, grid_y, cell_width, cell_height},
            {grid_x + cell_width, grid_y, cell_width, cell_height}
        };

        for (int i = 0; i < 4; ++i) {
            Canvas &canvas = _canvases[static_cast<size_t>(i + 1)];
            canvas.ensure_active_loaded();
            _renderer.render(canvas,
                             viewports[i][0],
                             viewports[i][1],
                             viewports[i][2],
                             viewports[i][3]);
        }

        _window.swap_buffers();
        _window.poll_events();
    }
}
