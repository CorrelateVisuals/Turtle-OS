#include "turtle_baseclass.h"

void TurtleBaseclass::ZoomEffect::run(bool is_on) {
  if (is_on) {
    if (_texture_start_scale > _texture_final_scale) {
      _texture_start_scale -= _zoom_speed;
    } else {
      _texture_start_scale = _texture_final_scale;
    }
  }
};

void TurtleBaseclass::ZoomEffect::getSettings() {
  std::cout << "ZoomEffect _scale_location: " << _scale_location
            << ",  _texture_start_scale: " << _texture_start_scale
            << ", _zoom_speed: " << _zoom_speed
            << ", _texture_final_scale:" << _texture_final_scale << std::endl
            << std::endl;
};