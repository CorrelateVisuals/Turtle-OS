#pragma once

#include <array>

class Rectangle {
public:
  std::array<float, 16> _vertices = {
      -0.5f, 0.5f,  0.0f, 1.0f, // Top-left
      -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
      0.5f,  -0.5f, 1.0f, 0.0f, // Bottom-right
      0.5f,  0.5f,  1.0f, 1.0f  // Top-right
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

struct Test {
  class Vertex {
  public:
    Vertex(std::array<float, 3> pos, std::array<float, 3> rot)
        : _position(pos), _rotation(rot) {
      print_config();
    };
    Vertex() { print_config(); };
    ~Vertex() = default;

    std::array<float, 3> get_position() { return _position; };
    std::array<float, 3> get_rotation() { return _rotation; };

  private:
    std::array<float, 3> _position{0.0f};
    std::array<float, 3> _rotation{0.0f};
    void print_config() {

      std::cout << "Vertex created at: " << std::endl
                << "x: " << _position[0] << "y: " << _position[1]
                << "z: " << _position[2] << std::endl
                << "x: " << _rotation[0] << "y: " << _rotation[1]
                << "z: " << _rotation[2] << std::endl;
    };
  };

  class Shape : public Vertex {
  public:
    Shape(int shape = 0, float scale = 1.0f) : _scale(scale), _shape(shape) {
      print_config();
    };

    ~Shape() = default;

    float get_scale() { return _scale; };
    int get_shape() { return _shape; };

  private:
    float _scale{1.0f};
    int _shape{0};

    void print_config() {
      std::cout << "Shape: " << _shape << " size of " << _scale << std::endl;
    };
  };

  class Texture : public Shape {
  public:
    Texture(std::string i) : _image(i){};
    std::string _image;
    std::array<std::string, 100> _images;
  };
};