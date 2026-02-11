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

Scene GalleryScene::compose(const std::string &folder, const std::string &top_left_path,
                            const std::string &bottom_left_path,
                            const std::array<float, 4> &color)
{
   return Scene{Image::folder(folder),
                Image::file(bottom_left_path, ImagePreset::bottom_left_fill()),
                Image::file(top_left_path, ImagePreset::top_left_quarter()),
                Image::color(color, ImagePreset::half_size_center())};
}

} // namespace turtle
