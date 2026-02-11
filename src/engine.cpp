

#include "engine.h"
#include "backend_GLSL.h"
#include "shader.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

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

   // Initialize canvas positions
   _win.set_canvas_start_x(0, -1.0f); _win.set_canvas_start_y(0, 1.0f); _win.set_canvas_start_w(0, 0.5f); _win.set_canvas_start_h(0, 0.5f);
   _win.set_canvas_start_x(1, 0.0f); _win.set_canvas_start_y(1, 1.0f); _win.set_canvas_start_w(1, 0.5f); _win.set_canvas_start_h(1, 0.5f);
   _win.set_canvas_start_x(2, -1.0f); _win.set_canvas_start_y(2, 0.5f); _win.set_canvas_start_w(2, 0.5f); _win.set_canvas_start_h(2, 0.5f);
   _win.set_canvas_start_x(3, 0.0f); _win.set_canvas_start_y(3, 0.5f); _win.set_canvas_start_w(3, 0.5f); _win.set_canvas_start_h(3, 0.5f);
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

         if (_win.get_resize_corner() == 0) { // TL: opposite BR fixed
            float fx = cx + cw; // BR x
            float fy = cy - ch; // BR y
            cx = nx;
            cy = ny;
            cw = fx - nx;
            ch = ny - fy;
         } else if (_win.get_resize_corner() == 1) { // TR: opposite BL fixed
            float fx = cx; // BL x
            float fy = cy - ch; // BL y
            cy = ny;
            cw = nx - fx;
            ch = ny - fy;
         } else if (_win.get_resize_corner() == 2) { // BL: opposite TR fixed
            float fx = cx + cw; // TR x
            float fy = cy; // TR y
            cx = nx;
            cw = fx - nx;
            ch = fy - ny;
         } else if (_win.get_resize_corner() == 3) { // BR: opposite TL fixed
            float fx = cx; // TL x
            float fy = cy; // TL y
            cw = nx - fx;
            ch = fy - ny;
         }

         _win.set_canvas_start_x(i, cx);
         _win.set_canvas_start_y(i, cy);
         _win.set_canvas_start_w(i, cw);
         _win.set_canvas_start_h(i, ch);

         update_overlay_quad(i, cx, cy, cw, ch);
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
   float z = overlay_z(index);
   switch (index) {
   case 0:
      _canvas1.set_quad(cx, cy, cw, ch, z);
      break;
   case 1:
      _canvas2.set_quad(cx, cy, cw, ch, z);
      break;
   case 2:
      _canvas3.set_quad(cx, cy, cw, ch, z);
      break;
   case 3:
      _canvas4.set_quad(cx, cy, cw, ch, z);
      break;
   default:
      break;
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
