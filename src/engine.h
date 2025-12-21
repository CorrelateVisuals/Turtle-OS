#pragma once

#include <GLES2/gl2.h>
#include <vector>

#include "turtle_baseclass.h"
#include "window.h"

// x, y,   u, v
constexpr std::array<float, 16> RECT_VERTICES = {
    -0.5f, 0.5f,  0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom-right
    0.5f,  0.5f,  1.0f, 1.0f  // Top-right
};

constexpr std::array<unsigned int, 6> INDICES = {0, 1, 2, 2, 3, 0};

class Engine {
public:
  Engine();
  ~Engine() = default;

  void run();

private:
  Window _window;

  TurtleBaseclass::ZoomEffect _zoom_effect{15.0f, 0.1f, 2.0f};

  GLuint _current_texture{0};
  std::vector<std::string> _textures;
  int _texture_index{0};

  void load_all_textures();
  void select_texture(int index);
  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);

  void create_shaders();
};