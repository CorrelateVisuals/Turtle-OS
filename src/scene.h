#pragma once

#include "image_stage.h"

#include <array>
#include <string>
#include <variant>
#include <vector>

namespace turtle
{

struct Image
{
   struct Folder
   {
      std::string folder;
      ImagePreset preset;
      float start_scale;
      float speed;
      float final_scale;
      float ease;
   };

   struct File
   {
      std::string path;
      ImagePreset preset;
   };

   struct Color
   {
      std::array<float, 4> rgba;
      ImagePreset preset;
   };

   using Definition = std::variant<Folder, File, Color>;

   static Image folder(std::string folder,
                       ImagePreset preset = ImagePreset::fit_screen(),
                       float start_scale = 15.0f, float speed = 0.1f,
                       float final_scale = 2.0f, float ease = 0.2f);
   static Image file(std::string path,
                     ImagePreset preset = ImagePreset::top_left_quarter());
   static Image color(std::array<float, 4> rgba,
                      ImagePreset preset = ImagePreset::half_size_center());

   const Definition &definition() const;

private:
   Definition _definition{};
};

using Scene = std::vector<Image>;

struct GalleryScene
{
   static Scene compose(const std::string &folder,
                        const std::string &top_left_path,
                        const std::string &bottom_left_path,
                        const std::array<float, 4> &color);
};

} // namespace turtle
