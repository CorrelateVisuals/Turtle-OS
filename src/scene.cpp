#include "scene.h"

#include <utility>

namespace turtle
{

Image Image::folder(std::string folder, ImagePreset preset, float start_scale, float speed,
                    float final_scale, float ease)
{
   Image image;
   image._definition = Folder{std::move(folder), preset, start_scale, speed, final_scale, ease};
   return image;
}

Image Image::file(std::string path, ImagePreset preset)
{
   Image image;
   image._definition = File{std::move(path), preset};
   return image;
}

Image Image::color(std::array<float, 4> rgba, ImagePreset preset)
{
   Image image;
   image._definition = Color{rgba, preset};
   return image;
}

const Image::Definition &Image::definition() const
{
   return _definition;
}

Scene::Scene(std::initializer_list<Image> init)
   : images(init)
{
}

} // namespace turtle
