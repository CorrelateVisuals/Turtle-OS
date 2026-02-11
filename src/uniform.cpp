#include "uniform.h"

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

} // namespace turtle
