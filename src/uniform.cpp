#include "uniform.h"

#include <iostream>

namespace turtle
{

Uniform *Uniform::_active_uniform = nullptr;
std::unordered_map<std::string, GLint> Uniform::_location_registry = {};

Uniform::Uniform(GLint location, float value)
   : _shader_location(location)
   , _value(value)
   , _location_source(nullptr)
   , _location_key()
{
}

float Uniform::get_uniform_value() const
{
   return _value;
}

void Uniform::set_uniform_value(float value)
{
   _value = value;
}

GLint Uniform::get_shader_location() const
{
   if (_location_source) {
      return _location_source->get_shader_location();
   }
   if (_shader_location == -1 && !_location_key.empty()) {
      auto iterator = _location_registry.find(_location_key);
      if (iterator != _location_registry.end()) {
         _shader_location = iterator->second;
      }
   }
   return _shader_location;
}

void Uniform::set_shader_location(GLint location)
{
   _location_source = nullptr;
   _location_key.clear();
   _shader_location = location;
}

void Uniform::bind_location_to(const Uniform &source)
{
   _location_source = &source;
   _location_key.clear();
}

void Uniform::set_location_key(const std::string &key)
{
   _location_source = nullptr;
   _location_key = key;
}

bool Uniform::has_shader_location() const
{
   return get_shader_location() != -1;
}

bool Uniform::apply_registered_location(const std::string &key)
{
   _location_source = nullptr;
   _location_key = key;
   auto iterator = _location_registry.find(key);
   if (iterator == _location_registry.end()) {
      return false;
   }
   _shader_location = iterator->second;
   return true;
}

void Uniform::register_location_value(const std::string &key, GLint location)
{
   _location_registry[key] = location;
}

Uniform *Uniform::get_active()
{
   return _active_uniform;
}

void Uniform::set_active(Uniform *uniform)
{
   _active_uniform = uniform;
}

ScaleVertices::ScaleVertices(float start, float speed, float final, float ease)
   : Uniform(-1, start)
   , _start_scale(start)
   , _scale(start)
   , _zoom_speed(speed)
   , _final_scale(final + speed)
   , _ease_factor(ease)
{
   set_uniform_value(_scale);
   log_settings();
}

void ScaleVertices::run(bool is_on)
{
   if (!is_on) {
      return;
   }

   if (_scale > _final_scale) {
      _scale -= _zoom_speed * (_scale - _final_scale) * _ease_factor;
   } else {
      _scale = _final_scale;
   }

   set_uniform_value(_scale);
}

void ScaleVertices::reset_start_scale()
{
   _scale = _start_scale;
   set_uniform_value(_start_scale);
}

void ScaleVertices::log_settings() const
{
   Uniform *active = Uniform::get_active();
   if (active != nullptr) {
      std::cout << "Zoom uniform seeded with location " << active->get_shader_location()
                << " and scale " << get_uniform_value() << '\n';
   }
}

} // namespace turtle
