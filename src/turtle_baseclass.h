#pragma once

#include <GLES2/gl2.h>

#include <array>
#include <vector>

#include "image_finder.h"

class TurtleBase {
public:
  TurtleBase(int argc, char *argv[]);
  ~TurtleBase() = default;

  //void run(){};

   class Effects {
  public:
    class ScaleVertices {
    public:
      ScaleVertices(float start, float speed, float final);
      ~ScaleVertices() = default;

      GLint _scale_location{0};
      float _reset_start_scale{0.0f};
      float _texture_start_scale{0.0f};
      float _zoom_speed{0.0f};
      float _texture_final_scale{0.0f};

      void run(bool is_on = true);
      void reset_start_scale();

    private:
      void getSettings();
    };
  };

  class Geometry {
  public:
    class Vertex {
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

    class Shape : public Vertex {
    public:
      Shape(int shape = 0, float scale = 1.0f,
            std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
            std::array<float, 3> rot = {0.0f, 0.0f, 0.0f},
            std::string name = "Shape Default");
      ~Shape() = default;

      float get_scale() { return _scale; };
      int get_shape() { return _shape; };

      std::array<float, 16> _vertices = {
          -0.5f, 0.5f,  0.0f, 1.0f, // Top-left
          -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
          0.5f,  -0.5f, 1.0f, 0.0f, // Bottom-right
          0.5f,  0.5f,  1.0f, 1.0f  // Top-right
      };

      std::array<unsigned int, 6> _indices = {0, 1, 2, 2, 3, 0};

    private:
      float _scale;
      int _shape;

      void print_config();
    };
  };

  class Image {
  public:
    class Texture : public Geometry::Shape {
    public:
      Texture(const std::string& folder);
      ~Texture() = default;

      GLuint _current_image;
      std::vector<std::string> _images;
      int _image_index;

      void select_texture(int index);

    private:
    GLuint load_texture(const std::string &path);
    void send_to_GPU_GLSL(GLuint& tex, unsigned char* pixels, const int width, const int height);
    void print_config();
    };
  };

protected:


private:
};