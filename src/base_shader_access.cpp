#include "base_shader_access.h"
#include "debug.h"

#include <iostream>

Uniform* Uniform::_active_uniform = nullptr;
std::unordered_map<std::string, GLint> Uniform::_location_registry{};

Uniform* Uniform::get_active() {
   return _active_uniform;
}

void Uniform::set_active(Uniform* u) {
   _active_uniform = u;
}

bool Uniform::apply_registered_location(const std::string &key)
{
   _location_source = nullptr;
   _location_key = key;
   auto it = _location_registry.find(key);
   if (it == _location_registry.end()) {
      return false;
   }
   _shader_location = it->second;
   return true;
}

void Uniform::register_location_value(const std::string &key, GLint location)
{
   _location_registry[key] = location;
}

ScaleVertices::ScaleVertices(float start, float speed, float final, float ease)
   : Uniform(0, start)
   , _start_scale{start}
   , _scale(start)
   , _zoom_speed(speed)
   , _final_scale(final + speed)
   , _ease_factor(ease)
{
   if (DEBUG_LEVEL == DebugLevel::BASIC) {
      std::cout << "Initializing Zoom." << std::endl;
   }
   set_uniform_value(_scale);
   getSettings();
}

void ScaleVertices::run(bool is_on)
{
   if (is_on) {
      if (_scale > _final_scale) {
         _scale -= _zoom_speed * (_scale - _final_scale) *
                   _ease_factor; // Adjust the factor as needed
      } else {
         _scale = _final_scale;
      }

      set_uniform_value(_scale);
   }
}

void ScaleVertices::reset_start_scale()
{
   _scale = _start_scale;
   set_uniform_value(_start_scale);
}

void ScaleVertices::getSettings()
{
   if (DEBUG_LEVEL == DebugLevel::BASIC) {
      std::cout << "Zoom _scale_location: " << _shader_location
                << ",  _scale: " << get_uniform_value() << ", _zoom_speed: " << _zoom_speed
                << ", _final_scale:" << _final_scale << std::endl
                << std::endl;
   }
}
