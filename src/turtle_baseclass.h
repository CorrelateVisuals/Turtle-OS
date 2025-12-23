// make all functions virtual (override)
#include <GLES2/gl2.h>

#include <iostream>
#include <vector>

class TurtleBase {
public:
  TurtleBase(int argc, char *argv[]);
  ~TurtleBase() = default;

  void run(){};

  class Shaders {
  public:
    static void create_shaders(GLint &scale_location);
  };

  class Image {
  public:
    Image() = default;
    ~Image() = default;

    class Zoom;

    class Texture {
    public:
      Texture() = default;
      ~Texture() = default;

      GLuint _current_image{0};
      std::vector<std::string> _images{};
      int _image_index{0};

      void select_texture(int index, Zoom &effect);
    };

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
        std::cout << _texture_start_scale << _reset_start_scale << std::endl;
        _texture_start_scale = _reset_start_scale;
      };

    private:
      void getSettings();
    };
  };

protected:
private:
};