#pragma once

#include "image_stage.h"
#include "render_stage.h"
#include "scene.h"

#include <array>
#include <memory>
#include <utility>
#include <vector>

namespace turtle
{

class Implementation
{
public:
   Implementation() = default;
   explicit Implementation(const Scene &scene);

   DynamicImageStage &image_folder(const std::string &folder,
                                   const ImagePreset &preset = ImagePreset::fit_screen(),
                                   float start_scale = 15.0f, float speed = 0.1f,
                                   float final_scale = 2.0f, float ease = 0.2f);
   StaticImageStage &image_file(const std::string &path,
                                const ImagePreset &preset = ImagePreset::top_left_quarter());
   SolidColorStage &image_color(const std::array<float, 4> &rgba,
                                const ImagePreset &preset = ImagePreset::half_size_center());

   void update();
   void render(GLFWwindow *window);
   DynamicImageStage *first_dynamic_stage() const;

private:
   RenderStage &add_image(const Image &image);
   DynamicImageStage &add_folder(const Image::Folder &folder);
   StaticImageStage &add_file(const Image::File &file);
   SolidColorStage &add_color(const Image::Color &color);

   template <typename Stage, typename... Args>
   Stage &emplace_stage(Args &&...args)
   {
      auto stage = std::make_unique<Stage>(std::forward<Args>(args)...);
      Stage &ref = *stage;
      _stages.emplace_back(std::move(stage));
      _manager.add_stage(ref);
      return ref;
   }

      StageManager _manager;
      std::vector<std::unique_ptr<RenderStage>> _stages;
      DynamicImageStage *_first_dynamic = nullptr;
};

} // namespace turtle
