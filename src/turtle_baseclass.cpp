#include "turtle_baseclass.h"

TurtleBase::Image::Zoom::Zoom(float start, float speed, float final)
    : _reset_start_scale{start}, _texture_start_scale{start},
      _zoom_speed(speed), _texture_final_scale(final + speed) {
  std::cout << "Initializing Zoom." << std::endl;
  getSettings();
};

void TurtleBase::Image::Zoom::run(bool is_on)
{

    if (is_on)
    {
        if (_texture_start_scale > _texture_final_scale)
        {
            _texture_start_scale -= _zoom_speed;
        }
        else
        {
            _texture_start_scale = _texture_final_scale;
        }
    }
};

void TurtleBase::Image::Zoom::getSettings() {
  std::cout << "Zoom _scale_location: " << _scale_location
            << ",  _texture_start_scale: " << _texture_start_scale
            << ", _zoom_speed: " << _zoom_speed
            << ", _texture_final_scale:" << _texture_final_scale << std::endl
            << std::endl;
};