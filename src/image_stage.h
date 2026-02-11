#pragma once

#include "render_stage.h"
#include "texture.h"
#include "scale_vertices.h"

#include <array>
#include <string>

namespace turtle
{

struct ImagePreset
{
   float x;
   float y;
   float width;
   float height;
   float depth;

   static ImagePreset fit_screen();
   static ImagePreset top_left_quarter();
   static ImagePreset bottom_left_fill();
   static ImagePreset half_size_center();
};

class DynamicImageStage : public RenderStage, public TextureArray
{
public:
   DynamicImageStage(const std::string &folder, const ImagePreset &preset,
                     float start_scale = 15.0f, float speed = 0.1f, float final = 2.0f,
                     float ease = 0.2f);

   void update() override;
   void render(GLFWwindow *window) override;

   void next_texture();
   void previous_texture();
   ScaleVertices &zoom();

private:
   ScaleVertices _zoom;
};

class StaticImageStage : public RenderStage, public Texture
{
public:
   StaticImageStage(const std::string &path, const ImagePreset &preset);

   void update() override;
   void render(GLFWwindow *window) override;
};

class SolidColorStage : public StaticImageStage
{
public:
   SolidColorStage(const std::array<float, 4> &color, const ImagePreset &preset);
};

} // namespace turtle
