

#include "engine.h"
#include "backend_GLSL.h"
#include "shader.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>

DebugLevel DEBUG_LEVEL = DebugLevel::BASIC;

namespace
{
struct CanvasBounds
{
   float left;
   float right;
   float bottom;
   float top;
};

CanvasBounds get_canvas_bounds(float cx, float cy, float cw, float ch)
{
   float left = std::min(cx, cx + cw);
   float right = std::max(cx, cx + cw);
   float bottom = std::min(cy - ch, cy);
   float top = std::max(cy - ch, cy);
   return {left, right, bottom, top};
}

constexpr float overlay_z(int index)
{
   return (index <= 1) ? 0.1f : 0.05f;
}

constexpr float overlay_grid_width = 1.32f;
constexpr float overlay_grid_height = overlay_grid_width * 6.0f / 19.0f;
constexpr float overlay_cell_width = overlay_grid_width * 0.5f;
constexpr float overlay_cell_height = overlay_grid_height * 0.5f;
}

Engine::Engine(int argc, char *argv[])
   : _win("Turtle-OS")
   , _main()
   , _canvas1()
   , _canvas2()
   , _canvas3()
   , _canvas4()
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
   _win.set_key_callback(Window::key_call_back);
   _win.set_mouse_button_callback(Window::mouse_button_call_back);
   _win.set_cursor_pos_callback(Window::cursor_pos_call_back);
   create_shaders(_main._zoom);
   glfwSetWindowUserPointer(_win.get_handle(), this);

   // Initialize canvas positions with a centered grid of 19:6 aspect ratio
   const float grid_left = -overlay_grid_width * 0.5f;
   const float grid_top = overlay_grid_height * 0.5f;
   const float grid_right = grid_left + overlay_cell_width;
   const float grid_center_row = grid_top - overlay_cell_height;

   _win.set_canvas_start_x(0, grid_left); _win.set_canvas_start_y(0, grid_top); _win.set_canvas_start_w(0, overlay_cell_width); _win.set_canvas_start_h(0, overlay_cell_height);
   _win.set_canvas_start_x(1, grid_right); _win.set_canvas_start_y(1, grid_top); _win.set_canvas_start_w(1, overlay_cell_width); _win.set_canvas_start_h(1, overlay_cell_height);
   _win.set_canvas_start_x(2, grid_left); _win.set_canvas_start_y(2, grid_center_row); _win.set_canvas_start_w(2, overlay_cell_width); _win.set_canvas_start_h(2, overlay_cell_height);
   _win.set_canvas_start_x(3, grid_right); _win.set_canvas_start_y(3, grid_center_row); _win.set_canvas_start_w(3, overlay_cell_width); _win.set_canvas_start_h(3, overlay_cell_height);

   update_overlay_quad(0, grid_left, grid_top, overlay_cell_width, overlay_cell_height);
   update_overlay_quad(1, grid_right, grid_top, overlay_cell_width, overlay_cell_height);
   update_overlay_quad(2, grid_left, grid_center_row, overlay_cell_width, overlay_cell_height);
   update_overlay_quad(3, grid_right, grid_center_row, overlay_cell_width, overlay_cell_height);
   sync_canvas4_to_overlay(_last_touched_overlay);
}

void Engine::handle_key(int key, int scancode, int action, int mods)
{
   if (action != GLFW_PRESS)
      return;

   if (key == GLFW_KEY_RIGHT) {
      _main.select_texture(_main.get_image_index() + 1);
      _main._zoom.reset_start_scale();
   } else if (key == GLFW_KEY_LEFT) {
      _main.select_texture(_main.get_image_index() - 1);
      _main._zoom.reset_start_scale();
   }

   if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(_win.get_handle(), true);
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
      int order[] = {1, 0, 3, 2};
      for (int idx : order) {
         float cx = _win.get_canvas_start_x(idx);
         float cy = _win.get_canvas_start_y(idx);
         float cw = _win.get_canvas_start_w(idx);
         float ch = _win.get_canvas_start_h(idx);

         auto bounds = get_canvas_bounds(cx, cy, cw, ch);

         if (nx >= bounds.left && nx <= bounds.right && ny >= bounds.bottom &&
             ny <= bounds.top) {
            float margin = 0.05f;
            bool near_left = nx >= bounds.left - margin && nx <= bounds.left + margin;
            bool near_right = nx >= bounds.right - margin && nx <= bounds.right + margin;
            bool near_top = ny >= bounds.top - margin && ny <= bounds.top + margin;
            bool near_bottom = ny >= bounds.bottom - margin && ny <= bounds.bottom + margin;

            if (near_left && near_top) {
               _win.set_resizing(true);
               _win.set_resize_corner(0); // TL
               _win.set_drag_canvas(idx);
            } else if (near_right && near_top) {
               _win.set_resizing(true);
               _win.set_resize_corner(1); // TR
               _win.set_drag_canvas(idx);
            } else if (near_left && near_bottom) {
               _win.set_resizing(true);
               _win.set_resize_corner(2); // BL
               _win.set_drag_canvas(idx);
            } else if (near_right && near_bottom) {
               _win.set_resizing(true);
               _win.set_resize_corner(3); // BR
               _win.set_drag_canvas(idx);
            } else {
               _win.set_dragging(true);
               _win.set_drag_canvas(idx);
            }
            _win.set_drag_start_x(nx);
            _win.set_drag_start_y(ny);
            sync_canvas4_to_overlay(idx);
            break;
         }
      }
   } else if (action == GLFW_RELEASE) {
      _win.set_dragging(false);
      _win.set_resizing(false);
      _win.set_drag_canvas(-1);
      _win.set_resize_corner(-1);
   }
}

void Engine::handle_cursor_pos(double xpos, double ypos)
{
   int width, height;
   glfwGetWindowSize(_win.get_handle(), &width, &height);

   float nx = (float)xpos / width * 2.0f - 1.0f;
   float ny = 1.0f - (float)ypos / height * 2.0f;

   if (_win.get_dragging() || _win.get_resizing()) {
      float dx = nx - _win.get_drag_start_x();
      float dy = ny - _win.get_drag_start_y();

      if (_win.get_resizing()) {
         // Resize based on corner, keeping opposite corner fixed, corner follows mouse
         int i = _win.get_drag_canvas();
         float cx = _win.get_canvas_start_x(i);
         float cy = _win.get_canvas_start_y(i);
         float cw = _win.get_canvas_start_w(i);
         float ch = _win.get_canvas_start_h(i);

         float left = std::min(cx, cx + cw);
         float right = std::max(cx, cx + cw);
         float top = std::max(cy, cy - ch);
         float bottom = std::min(cy, cy - ch);

         switch (_win.get_resize_corner()) {
         case 0: // TL
            left = nx;
            top = ny;
            break;
         case 1: // TR
            right = nx;
            top = ny;
            break;
         case 2: // BL
            left = nx;
            bottom = ny;
            break;
         case 3: // BR
            right = nx;
            bottom = ny;
            break;
         default:
            break;
         }

         if (right < left) {
            std::swap(right, left);
         }
         if (top < bottom) {
            std::swap(top, bottom);
         }

         float new_cw = right - left;
         float new_ch = top - bottom;
         float new_cx = left;
         float new_cy = top;

         _win.set_canvas_start_x(i, new_cx);
         _win.set_canvas_start_y(i, new_cy);
         _win.set_canvas_start_w(i, new_cw);
         _win.set_canvas_start_h(i, new_ch);

         update_overlay_quad(i, new_cx, new_cy, new_cw, new_ch);
      } else if (_win.get_dragging()) {
         // Drag
         int i = _win.get_drag_canvas();
         float cx = _win.get_canvas_start_x(i) + dx;
         float cy = _win.get_canvas_start_y(i) + dy;

         _win.set_canvas_start_x(i, cx);
         _win.set_canvas_start_y(i, cy);

         update_overlay_quad(i, cx, cy, _win.get_canvas_start_w(i),
                             _win.get_canvas_start_h(i));

         _win.set_drag_start_x(nx);
         _win.set_drag_start_y(ny);
      }
   }
}

void Engine::update_overlay_quad(int index, float cx, float cy, float cw, float ch)
{
   OverlayCanvas &canvas = overlay_by_index(index);
   canvas.set_quad(cx, cy, cw, ch, overlay_z(index));
}

Engine::OverlayCanvas &Engine::overlay_by_index(int index)
{
   switch (index) {
   case 0:
      return _canvas1;
   case 1:
      return _canvas2;
   case 2:
      return _canvas3;
   default:
      return _canvas4;
   }
}

void Engine::sync_canvas4_to_overlay(int source_index)
{
   if (source_index < 0 || source_index > 3) {
      return;
   }
   const OverlayCanvas &source = overlay_by_index(source_index);
   _last_touched_overlay = source_index;
   if (source.is_color()) {
      _canvas4.set_solid_color(source.get_color());
   } else {
      if (!source.get_path().empty()) {
         _canvas4.set_texture_path(source.get_path());
      }
      _canvas4.set_color_value(source.get_color());
   }
}

void Engine::run()
{
   while (!_win.should_close()) {
      GLSL::render_loop(_win.get_handle(), _main, _main._zoom);
      _main._zoom.run();

      GLSL::render_loop(_win.get_handle(), _canvas3, &_canvas3.uniform());
      GLSL::render_loop(_win.get_handle(), _canvas4, &_canvas4.uniform());
      GLSL::render_loop(_win.get_handle(), _canvas1, &_canvas1.uniform());
      GLSL::render_loop(_win.get_handle(), _canvas2, &_canvas2.uniform());

      // Swap buffers and poll events
      _win.swap_buffers();
      _win.poll_events();
   }
};
