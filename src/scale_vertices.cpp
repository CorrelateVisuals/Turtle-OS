#include "scale_vertices.h"

namespace turtle
{

ScaleVertices::ScaleVertices(float start, float speed, float final, float ease)
   : Uniform(-1, start)
   , _start_scale(start)
   , _scale(start)
   , _zoom_speed(speed)
   , _final_scale(final + speed)
   , _ease_factor(ease)
{
   set_uniform_value(_scale);
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

} // namespace turtle
