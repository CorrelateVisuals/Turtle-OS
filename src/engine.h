#pragma once

#include <GLES2/gl2.h>
#include <array>
#include <vector>

#include "turtle_baseclass.h"
#include "window.h"

class Engine : public TurtleBase {
public:
  Engine(int argc, char *argv[]);
  ~Engine() = default;

  void run();

private:
  class Composition : public TurtleBase::Composition {
  public:
    TurtleBase::Image::Zoom _zoom_effect{15.0f, 0.1f, 2.0f};
    TurtleBase::Image::Texture _texture{};
  };

private:
  Window _win;
  Composition _comp;

  static void key_call_back(GLFWwindow *window, int key, int seconds,
                            int action, int mods);
};

struct Texture {
public:
  GLuint _current_image{0};
  std::vector<std::string> _images{};
  int _image_index{0};

  void select_texture(int index, Zoom &effect);
};

class Effect {
public:
};

class Zoom : public Effect {
public:
  Zoom(float start, float speed, float final);
  ~Zoom() = default;

  GLint _scale_location{0};
  float _reset_start_scale{0.0f};
  float _texture_start_scale{0.0f};
  float _zoom_speed{0.0f};
  float _texture_final_scale{0.0f};

  void run(bool is_on = true);
  void reset_start_scale() {
    std::cout << _texture_start_scale << _reset_start_scale << std::endl;
    _texture_start_scale = _reset_start_scale;
  };

private:
  void getSettings();
};

class Shape {
public:
  Shape(int shape);
  ~Shape() = default;
};

class Image : public Shape {
public:
  Image(int shape, std::string texture) : Shape(shape) {}
  Texture _media;

private:
  void zoom(){};
  void switch_source(){};
};

Image _image;
_image.zoom();

class Media {
public:
  Media(std::string source);
};

class Image : public Media {
  Image(std::string source) : Media(source) {}
  Shape _shape{0};
};

Image _img;

enum shapes = {rect, circle, cube};
Image()