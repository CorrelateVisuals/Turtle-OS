#pragma once

#include "uniform.h"

namespace turtle
{

class ScaleVertices : public Uniform
{
public:
   ScaleVertices(float start, float speed, float final, float ease);
   ~ScaleVertices() override = default;

   void run(bool is_on = true);
   void reset_start_scale();

private:
   float _start_scale;
   float _scale;
   float _zoom_speed;
   float _final_scale;
   float _ease_factor;
};

} // namespace turtle
