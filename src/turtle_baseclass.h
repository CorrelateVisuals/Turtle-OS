#pragma once

#include <GLES2/gl2.h>

#include <array>
#include <iostream>
#include <vector>

#include <geometry.h>


class TurtleBase {
public:
  TurtleBase(int argc, char *argv[]);
  ~TurtleBase() = default;

  void run(){};

  class Composition {
  public:
    std::array<float, 3> _position{0.0f, 0.0f, 0.0f};
    std::array<float, 3> _rotation{0.0f, 0.0f, 0.0f};
  };

  class Image : public Composition {
  public:
    Image() = default;
    ~Image() = default;

    class Zoom;

    // class Texture : public Rectangle {
    // public:
    //   Texture() = default;
    //   ~Texture() = default;

    //   GLuint _current_image{0};
    //   std::vector<std::string> _images{};
    //   int _image_index{0};

    //   // void select_texture(int index);
    //   // GLuint load_texture(const std::string &path);
    // };

    class Zoom {
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
        //std::cout << _texture_start_scale << _reset_start_scale << std::endl;
        _texture_start_scale = _reset_start_scale;
      };

    private:
      void getSettings();
    };
  };
  

protected:
private:
};