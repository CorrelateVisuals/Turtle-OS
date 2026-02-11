#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <unordered_map>

namespace turtle
{

class Uniform
{
public:
   Uniform(GLint location = -1, float value = 0.0f);
   virtual ~Uniform() = default;

   float get_uniform_value() const;
   void set_uniform_value(float value);

   GLint get_shader_location() const;
   void set_shader_location(GLint location);

   void bind_location_to(const Uniform &source);
   void set_location_key(const std::string &key);
   bool has_shader_location() const;
   bool apply_registered_location(const std::string &key);
   static void register_location_value(const std::string &key, GLint location);

   static Uniform *get_active();
   static void set_active(Uniform *uniform);

protected:
   mutable GLint _shader_location;
   float _value;
   const Uniform *_location_source;
   std::string _location_key;

private:
   static Uniform *_active_uniform;
   static std::unordered_map<std::string, GLint> _location_registry;
};

} // namespace turtle
