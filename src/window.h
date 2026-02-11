#pragma once

// #include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Window
{
public:
   Window(std::string_view title);
   ~Window();

   bool should_close() const;
   void swap_buffers();
   void poll_events();
   void set_key_callback(GLFWkeyfun callback);
   void set_mouse_button_callback(GLFWmousebuttonfun callback);
   void set_cursor_pos_callback(GLFWcursorposfun callback);

   GLFWwindow *get_handle() const;

   // Mouse handling getters and setters
   bool get_dragging() const { return _dragging; }
   void set_dragging(bool d) { _dragging = d; }
   int get_drag_canvas() const { return _drag_canvas; }
   void set_drag_canvas(int c) { _drag_canvas = c; }
   bool get_resizing() const { return _resizing; }
   void set_resizing(bool r) { _resizing = r; }
   int get_resize_corner() const { return _resize_corner; }
   void set_resize_corner(int c) { _resize_corner = c; }
   double get_drag_start_x() const { return _drag_start_x; }
   void set_drag_start_x(double x) { _drag_start_x = x; }
   double get_drag_start_y() const { return _drag_start_y; }
   void set_drag_start_y(double y) { _drag_start_y = y; }
   float get_canvas_start_x(int i) const { return _canvas_start_x[i]; }
   void set_canvas_start_x(int i, float v) { _canvas_start_x[i] = v; }
   float get_canvas_start_y(int i) const { return _canvas_start_y[i]; }
   void set_canvas_start_y(int i, float v) { _canvas_start_y[i] = v; }
   float get_canvas_start_w(int i) const { return _canvas_start_w[i]; }
   void set_canvas_start_w(int i, float v) { _canvas_start_w[i] = v; }
   float get_canvas_start_h(int i) const { return _canvas_start_h[i]; }
   void set_canvas_start_h(int i, float v) { _canvas_start_h[i] = v; }

   static void key_call_back(GLFWwindow *window, int key, int scancode, int action, int mods);
   static void mouse_button_call_back(GLFWwindow *window, int button, int action, int mods);
   static void cursor_pos_call_back(GLFWwindow *window, double xpos, double ypos);

private:
   GLFWwindow *_handle = nullptr;

   // Mouse handling
   bool _dragging = false;
   int _drag_canvas = -1; // 0: front, 1: lower, 2: front2
   bool _resizing = false;
   int _resize_corner = -1; // 0: TL, 1: TR, 2: BL, 3: BR
   double _drag_start_x = 0.0, _drag_start_y = 0.0;
   float _canvas_start_x[3] = {0.0f}, _canvas_start_y[3] = {0.0f}, _canvas_start_w[3] = {0.0f}, _canvas_start_h[3] = {0.0f};
};
