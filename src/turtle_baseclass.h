// make all functions virtual (override)
#include <GLES2/gl2.h>

#include <iostream>

class TurtleBaseclass
{
public:
  TurtleBaseclass() = default;
  ~TurtleBaseclass() = default;

  void run() {};

  class ZoomEffect
  {
  public:
    ZoomEffect(float start, float speed, float final) : _reset_start_scale{start}, _texture_start_scale{start}, _zoom_speed(speed),
                                                        _texture_final_scale(final + speed)
    {
      std::cout << "Initializing ZoomEffect." << std::endl;
      getSettings();
    };

    GLint _scale_location{0};
    float _reset_start_scale{0.0f};
    float _texture_start_scale{0.0f};
    float _zoom_speed{0.0f};
    float _texture_final_scale{0.0f};

    void run(bool is_on = true);
    void reset_start_scale()
    {
      std::cout << _texture_start_scale << _reset_start_scale << std::endl;
      _texture_start_scale = _reset_start_scale;
    };

  private:
    void getSettings();
  };

protected:
private:
};