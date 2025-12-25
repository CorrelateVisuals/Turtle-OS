#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>

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
}
;

#include <array>

class Vertex {
public:
  Vertex() = default;
  ~Vertex() = default;

  void get_position(){};
  void get_rotation(){};

private:
  std::array<float, 3> _position{0.0f};
  std::array<float, 3> _rotation{0.0f};
};

class Shape : public Vertex {
public:
  Shape(int s) : _shape(s) {}
  ~Shape() = default;

  void get_scale(){};
  void get_shape(){};

private:
  float scale{1.0f};
  int _shape{0};
};

// a shape can have an image...

class Image {
public:
  Image(std::string path){};
};

class Texture : public Shape {
public:
  Texture(int s, std::string p) : Shape(s), _image(p) {}
  Texture() = default;

  Image _image;
};
