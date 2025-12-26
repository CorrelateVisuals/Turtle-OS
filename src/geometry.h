#pragma once

#include <GLES2/gl2.h>

#include <array>
#include <string>
#include <iostream>
#include <vector>

class Rectangle
{
public:
  std::array<float, 16> _vertices = {
      -0.5f, 0.5f, 0.0f, 1.0f,  // Top-left
      -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
      0.5f, -0.5f, 1.0f, 0.0f,  // Bottom-right
      0.5f, 0.5f, 1.0f, 1.0f    // Top-right
  };

  std::array<unsigned int, 6> _indices = {0, 1, 2, 2, 3, 0};
};

// #include <array>
// #include <iostream>
// #include <string>

// enum class Geo : unsigned int {
//   Rectangle = 0,
// };

// struct GeometryData {
//     std::array<float, 16> vertices;   // 4 vertices × (x,y,u,v)
//     std::array<unsigned int, 6> indices;
// };

// inline GeometryData getGeometry(Geo g) {
//   switch (g) {
//   case Geo::Rectangle: {
//     return GeometryData{                           // clang-format off
//                 { -0.5f,  0.5f, 0.0f, 1.0f,   // Top‑left
//                   -0.5f, -0.5f, 0.0f, 0.0f,   // Bottom‑left
//                    0.5f, -0.5f, 1.0f, 0.0f,   // Bottom‑right
//                    0.5f,  0.5f, 1.0f, 1.0f } // Top‑right
//                                                    // clang-format on
//                         ,
//                         {0, 1, 2, 2, 3, 0}};
//   };
//   };
// };

struct Geometry
{
private:
public:
  class Vertex
  {
  public:
    Vertex(std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
           std::array<float, 3> rot = {0.0f, 0.0f, 0.0f},
           std::string name = "Vertex Default");
    virtual ~Vertex() = default;

    std::array<float, 3> get_position() { return _position; };
    std::array<float, 3> get_rotation() { return _rotation; };
    std::string get_name() { return _name; };

  private:
    std::array<float, 3> _position;
    std::array<float, 3> _rotation;
    std::string _name;

    void print_config();
  };

  class Shape : public Vertex
  {
  public:
    Shape(int shape = 0, float scale = 1.0f, std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
          std::array<float, 3> rot = {0.0f, 0.0f, 0.0f},
          std::string name = "Shape Default");
    ~Shape() = default;

    float get_scale() { return _scale; };
    int get_shape() { return _shape; };

    std::array<float, 16> _vertices = {
        -0.5f, 0.5f, 0.0f, 1.0f,  // Top-left
        -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, 1.0f, 1.0f    // Top-right
    };

    std::array<unsigned int, 6> _indices = {0, 1, 2, 2, 3, 0};

  private:
    float _scale;
    int _shape;

    void print_config();
  };
};

class Img
{
public:
  class Zoom;

  class Texture : public Geometry::Shape
  {
  public:
    Texture() : Shape(0, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "Texture Default"), _current_image(0), _images(), _image_index(0) { print_config(); };
    ~Texture() = default;

    GLuint _current_image;
    std::vector<std::string> _images;
    int _image_index;

    void select_texture(int index);
    GLuint load_texture(const std::string &path);

  private:
    void print_config();
  };
};