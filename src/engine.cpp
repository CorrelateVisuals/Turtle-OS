

#include "engine.h"
#include "backend_GLSL.h"
#include "shader.h"

#include <filesystem>
#include <iostream>
#include <string>

DebugLevel DEBUG_LEVEL = DebugLevel::BASIC;

Engine::Engine(int argc, char *argv[])
   : _win("Turtle-OS")
   , _main()
   , _front()
   , _front2()
   , _lower()
{
   DEBUG_LEVEL = DebugLevel::DETAILED;
   for (int i = 1; i < argc; ++i) {
      std::string argument = argv[i];
      if (argument == "turtle") {
         if (DEBUG_LEVEL != DebugLevel::OFF) {
            std::cout << "TurtleBase found!\n" << std::endl;
         }
         break;
      }
   }
   _win.set_key_callback(key_call_back);
   _win.set_mouse_button_callback(Window::mouse_button_call_back);
   _win.set_cursor_pos_callback(Window::cursor_pos_call_back);
   create_shaders(_main._zoom);
   glfwSetWindowUserPointer(_win.get_handle(), this);

   // Initialize canvas positions
   _canvas_start_x[0] = -1.0f; _canvas_start_y[0] = 1.0f; _canvas_start_w[0] = 0.5f; _canvas_start_h[0] = 0.5f;
   _canvas_start_x[1] = -1.0f; _canvas_start_y[1] = 0.5f; _canvas_start_w[1] = 0.5f; _canvas_start_h[1] = 1.5f;
   _canvas_start_x[2] = 0.0f; _canvas_start_y[2] = 1.0f; _canvas_start_w[2] = 0.5f; _canvas_start_h[2] = 0.5f;
}

void Engine::key_call_back(GLFWwindow *window, int key, int, int action, int)
{
   if (action != GLFW_PRESS)
      return;

   // Retrieve Engine* pointer stored in GLFW window
   Engine *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

   if (!engine)
      return;

   if (key == GLFW_KEY_RIGHT) {
      engine->_main.select_texture(engine->_main.get_image_index() + 1);
      engine->_main._zoom.reset_start_scale();
   } else if (key == GLFW_KEY_LEFT) {
      engine->_main.select_texture(engine->_main.get_image_index() - 1);
      engine->_main._zoom.reset_start_scale();
   }

   if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, true);
   }
}

void Engine::handle_mouse_button(int button, int action, int mods)
{
   if (button != GLFW_MOUSE_BUTTON_LEFT) return;

   double xpos, ypos;
   glfwGetCursorPos(_win.get_handle(), &xpos, &ypos);

   int width, height;
   glfwGetWindowSize(_win.get_handle(), &width, &height);

   float nx = (float)xpos / width * 2.0f - 1.0f;
   float ny = 1.0f - (float)ypos / height * 2.0f;

   if (action == GLFW_PRESS) {
      // Check which canvas and if corner
      for (int i = 0; i < 3; ++i) {
         float cx = _canvas_start_x[i];
         float cy = _canvas_start_y[i];
         float cw = _canvas_start_w[i];
         float ch = _canvas_start_h[i];

         // Check if inside canvas
         if (nx >= cx && nx <= cx + cw && ny >= cy - ch && ny <= cy) {
            float margin = 0.05f; // corner size
            if (nx >= cx - margin && nx <= cx + margin &&
                ny >= cy - margin && ny <= cy + margin) {
               _resizing = true;
               _resize_corner = 0; // TL
               _drag_canvas = i;
            } else if (nx >= cx + cw - margin && nx <= cx + cw + margin &&
                       ny >= cy - margin && ny <= cy + margin) {
               _resizing = true;
               _resize_corner = 1; // TR
               _drag_canvas = i;
            } else if (nx >= cx - margin && nx <= cx + margin &&
                       ny >= cy - ch - margin && ny <= cy - ch + margin) {
               _resizing = true;
               _resize_corner = 2; // BL
               _drag_canvas = i;
            } else if (nx >= cx + cw - margin && nx <= cx + cw + margin &&
                       ny >= cy - ch - margin && ny <= cy - ch + margin) {
               _resizing = true;
               _resize_corner = 3; // BR
               _drag_canvas = i;
            } else {
               _dragging = true;
               _drag_canvas = i;
            }
            _drag_start_x = nx;
            _drag_start_y = ny;
            break;
         }
      }
   } else if (action == GLFW_RELEASE) {
      _dragging = false;
      _resizing = false;
      _drag_canvas = -1;
      _resize_corner = -1;
   }
}

void Engine::handle_cursor_pos(double xpos, double ypos)
{
   int width, height;
   glfwGetWindowSize(_win.get_handle(), &width, &height);

   float nx = (float)xpos / width * 2.0f - 1.0f;
   float ny = 1.0f - (float)ypos / height * 2.0f;

   if (_dragging || _resizing) {
      float dx = nx - _drag_start_x;
      float dy = ny - _drag_start_y;

      if (_resizing) {
         // Resize based on corner, keeping opposite corner fixed
         float &cx = _canvas_start_x[_drag_canvas];
         float &cy = _canvas_start_y[_drag_canvas];
         float &cw = _canvas_start_w[_drag_canvas];
         float &ch = _canvas_start_h[_drag_canvas];

         if (_resize_corner == 0) { // TL: move left and top
            cx += dx;
            cw -= dx;
            cy -= dy;
            ch += dy;
         } else if (_resize_corner == 1) { // TR: move right and top
            cw += dx;
            cy -= dy;
            ch += dy;
         } else if (_resize_corner == 2) { // BL: move left and bottom
            cx += dx;
            cw -= dx;
            ch -= dy;
         } else if (_resize_corner == 3) { // BR: move right and bottom
            cw += dx;
            ch -= dy;
         }

         // Update the quad
         if (_drag_canvas == 0) {
            _front.set_quad(cx, cy, cw, ch, 0.1f);
         } else if (_drag_canvas == 1) {
            _lower.set_quad(cx, cy, cw, ch, 0.05f);
         } else if (_drag_canvas == 2) {
            _front2.set_quad(cx, cy, cw, ch, 0.1f);
         }
      } else if (_dragging) {
         // Drag
         float &cx = _canvas_start_x[_drag_canvas];
         float &cy = _canvas_start_y[_drag_canvas];

         cx += dx;
         cy += dy;

         if (_drag_canvas == 0) {
            _front.set_quad(cx, cy, _canvas_start_w[0], _canvas_start_h[0], 0.1f);
         } else if (_drag_canvas == 1) {
            _lower.set_quad(cx, cy, _canvas_start_w[1], _canvas_start_h[1], 0.05f);
         } else if (_drag_canvas == 2) {
            _front2.set_quad(cx, cy, _canvas_start_w[2], _canvas_start_h[2], 0.1f);
         }

         _drag_start_x = nx;
         _drag_start_y = ny;
      }
   }
}

void Engine::run()
{
   while (!_win.should_close()) {
      GLSL::render_loop(_win.get_handle(), _main, _main._zoom);
      _main._zoom.run();

      GLSL::render_loop(_win.get_handle(), _lower, &_lower.uniform());
      GLSL::render_loop(_win.get_handle(), _front, &_front.uniform());
      GLSL::render_loop(_win.get_handle(), _front2, &_front2.uniform());

      // Swap buffers and poll events
      _win.swap_buffers();
      _win.poll_events();
   }
};
