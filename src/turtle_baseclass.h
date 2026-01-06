#pragma once

#include <GLES2/gl2.h>

#include <array>

#include "image_finder.h"

class TurtleBase
{
public:
  TurtleBase(int argc, char *argv[]);
  ~TurtleBase() = default;

  class Uniform
  {
  public:
    Uniform(GLint l = 0, float v = 0.0f) : _shader_location(l), _value(v) {};
    ~Uniform() = default;

    float get_uniform_value() { return _value; };
    void set_uniform_value(float v) { _value = v; };

    GLint _shader_location;

  private:
    float _value;
  };

  class ScaleVertices : public Uniform
  {
  public:
    ScaleVertices(float start, float speed, float final, float ease);
    ~ScaleVertices() = default;

    float _start_scale;
    float _scale;
    float _zoom_speed;
    float _final_scale;
    float _ease_factor;

    void run(bool is_on = true);
    void reset_start_scale();

  private:
    void getSettings();
  };

  class Geometry
  {
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
      Shape(int shape = 0, float scale = 1.0f,
            std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
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

  class Image
  {
  public:
    class TextureArray : public Geometry::Shape
    {
    public:
      TextureArray(const std::string &folder);
      ~TextureArray() = default;

      GLuint _current_image;
      std::array<std::string, TEXTURE_ARRAY_SIZE> _images;
      int _image_index;

      void select_texture(int index);

    private:
      void load_texture(const std::string &path);
      void print_config();
    };
  };

protected:
private:
};