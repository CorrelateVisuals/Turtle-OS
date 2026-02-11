#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <unordered_map>

class Uniform
{
public:
   Uniform(GLint l = -1, float v = 0.0f)
      : _shader_location(l)
      , _value(v)
      , _location_source(nullptr)
      , _location_key()
   {
   }
   ~Uniform() = default;

   float get_uniform_value() const { return _value; };
   void set_uniform_value(float v) { _value = v; };

   GLint get_shader_location() const
   {
      if (_location_source) {
         return _location_source->get_shader_location();
      }
      if (_shader_location == -1 && !_location_key.empty()) {
         auto it = _location_registry.find(_location_key);
         if (it != _location_registry.end()) {
            _shader_location = it->second;
         }
      }
      return _shader_location;
   };
   void set_shader_location(GLint l)
   {
      _location_source = nullptr;
      _shader_location = l;
      _location_key.clear();
   };

   void bind_location_to(const Uniform &source)
   {
      _location_source = &source;
      _location_key.clear();
   }
   void set_location_key(const std::string &key)
   {
      _location_source = nullptr;
      _location_key = key;
   }
   bool has_shader_location() const { return get_shader_location() != -1; }
   bool apply_registered_location(const std::string &key);
   static void register_location_value(const std::string &key, GLint location);

   static Uniform *get_active();
   static void set_active(Uniform *u);

protected:
   mutable GLint _shader_location;
   float _value;
   const Uniform *_location_source;
   std::string _location_key;
   static Uniform *_active_uniform;
   static std::unordered_map<std::string, GLint> _location_registry;
};

class ScaleVertices : public Uniform
{
public:
   ScaleVertices(float start, float speed, float final, float ease);
   ~ScaleVertices() = default;

   void run(bool is_on = true);
   void reset_start_scale();

   void set_shader_location(GLint l) { _shader_location = l; }

private:
   void getSettings();

   float _start_scale;
   float _scale;
   float _zoom_speed;
   float _final_scale;
   float _ease_factor;
};
