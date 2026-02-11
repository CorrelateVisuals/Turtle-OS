#include "image_stage.h"

#include "backend_GLSL.h"

#include <algorithm>
#include <array>

namespace turtle
{

namespace
{
constexpr unsigned char clamp_color(float channel)
{
   return static_cast<unsigned char>(std::clamp(channel, 0.0f, 1.0f) * 255.0f);
}
} // namespace

ImagePreset ImagePreset::fit_screen()
{
   return {-1.0f, 1.0f, 2.0f, 2.0f, 0.0f};
}

ImagePreset ImagePreset::top_left_quarter()
{
   return {-1.0f, 1.0f, 0.5f, 0.5f, 0.1f};
}

ImagePreset ImagePreset::bottom_left_fill()
{
   return {-1.0f, 0.5f, 0.5f, 1.5f, 0.05f};
}

ImagePreset ImagePreset::half_size_center()
{
   return {-0.5f, 0.5f, 1.0f, 1.0f, 0.15f};
}

DynamicImageStage::DynamicImageStage(const std::string &folder, const ImagePreset &preset,
                                     float start_scale, float speed, float final, float ease)
   : TextureArray(folder)
   , _zoom(start_scale, speed, final, ease)
{
   set_quad(preset.x, preset.y, preset.width, preset.height, preset.depth);
}

void DynamicImageStage::update()
{
   _zoom.run();
}

void DynamicImageStage::render(GLFWwindow *window)
{
   GLSL::render_loop(window, *this, _zoom);
}

void DynamicImageStage::next_texture()
{
   select_texture(get_image_index() + 1);
   _zoom.reset_start_scale();
}

void DynamicImageStage::previous_texture()
{
   select_texture(get_image_index() - 1);
   _zoom.reset_start_scale();
}

ScaleVertices &DynamicImageStage::zoom()
{
   return _zoom;
}

StaticImageStage::StaticImageStage(const std::string &path, const ImagePreset &preset)
   : Texture(path)
{
   set_quad(preset.x, preset.y, preset.width, preset.height, preset.depth);
}

void StaticImageStage::update()
{
}

void StaticImageStage::render(GLFWwindow *window)
{
   GLSL::render_loop(window, *this, &uniform());
}

SolidColorStage::SolidColorStage(const std::array<float, 4> &color, const ImagePreset &preset)
   : StaticImageStage("", preset)
{
   set_color(color);
}

void SolidColorStage::set_color(const std::array<float, 4> &color)
{
   _color = color;
   unsigned char pixel[4] = {clamp_color(color[0]), clamp_color(color[1]),
                             clamp_color(color[2]), clamp_color(color[3])};
   upload_pixels(pixel, 1, 1);
}

const std::array<float, 4> &SolidColorStage::color() const
{
   return _color;
}

} // namespace turtle
