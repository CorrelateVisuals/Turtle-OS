#include "implementation.h"

#include <GLFW/glfw3.h>

#include <type_traits>

namespace turtle
{

Implementation::Implementation(const Scene &scene)
{
   for (const auto &image : scene) {
      add_image(image);
   }
}

DynamicImageStage &Implementation::image_folder(const std::string &folder,
                                                const ImagePreset &preset,
                                                float start_scale, float speed,
                                                float final_scale, float ease)
{
   return add_folder(Image::Folder{folder, preset, start_scale, speed, final_scale, ease});
}

StaticImageStage &Implementation::image_file(const std::string &path,
                                             const ImagePreset &preset)
{
   return add_file(Image::File{path, preset});
}

SolidColorStage &Implementation::image_color(const std::array<float, 4> &rgba,
                                             const ImagePreset &preset)
{
   return add_color(Image::Color{rgba, preset});
}

RenderStage &Implementation::add_image(const Image &image)
{
   return std::visit(
      [this](const auto &cfg) -> RenderStage & {
         using Spec = std::decay_t<decltype(cfg)>;
         if constexpr (std::is_same_v<Spec, Image::Folder>) {
            return add_folder(cfg);
         }
         else if constexpr (std::is_same_v<Spec, Image::File>) {
            return add_file(cfg);
         }
         else {
            return add_color(cfg);
         }
      },
      image.definition());
}

DynamicImageStage &Implementation::add_folder(const Image::Folder &folder)
{
   DynamicImageStage &stage =
      emplace_stage<DynamicImageStage>(folder.folder, folder.preset, folder.start_scale,
                                       folder.speed, folder.final_scale, folder.ease);
   if (!_first_dynamic)
      _first_dynamic = &stage;
   return stage;
}

StaticImageStage &Implementation::add_file(const Image::File &file)
{
   return emplace_stage<StaticImageStage>(file.path, file.preset);
}

SolidColorStage &Implementation::add_color(const Image::Color &color)
{
   SolidColorStage &stage = emplace_stage<SolidColorStage>(color.rgba, color.preset);
   _color_stages.push_back(&stage);
   return stage;
}

void Implementation::update()
{
   _manager.update();
}

void Implementation::render(GLFWwindow *window)
{
   _manager.render(window);
}

DynamicImageStage *Implementation::first_dynamic_stage() const
{
   return _first_dynamic;
}

SolidColorStage *Implementation::first_color_stage() const
{
   if (_color_stages.empty())
      return nullptr;
   return _color_stages.front();
}

} // namespace turtle
