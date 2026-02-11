#include "texture.h"

#include "backend_GLSL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace turtle
{

Texture::Texture(const std::string &path, const std::string &location_key, float uniform_value)
   : Shape(0, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "Texture")
   , _image(0)
   , _path(path)
{
   if (!location_key.empty()) {
      uniform().set_location_key(location_key);
   }
   uniform().set_uniform_value(uniform_value);

   if (!path.empty()) {
      load_texture(path);
   }
}

Texture::Texture()
   : Texture("", "scale", 1.0f)
{
}

std::string Texture::get_path() const
{
   return _path;
}

GLuint Texture::get_image() const
{
   return _image;
}

void Texture::load_texture(const std::string &path)
{
   stbi_set_flip_vertically_on_load(true);

   int width = 0;
   int height = 0;
   int channels = 0;
   unsigned char *pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);

   if (!pixels) {
      std::cerr << "Failed to load image: " << path << '\n';
      return;
   }

   GLSL::texture_to_gpu(_image, pixels, width, height);

   stbi_image_free(pixels);
}

TextureArray::TextureArray(const std::string &folder)
   : Texture("", "scale", 1.0f)
   , _finder()
   , _images()
   , _image_index(0)
{
   _finder.set_root(folder);
   _finder.find_all_images(_images);
   _finder.randomize_order(_images);

   size_t filled = _finder.get_filled_array_count(_images);
   if (filled > 0) {
      load_texture(_images[0]);
      _path = _images[0];
   }
}

void TextureArray::select_texture(int index)
{
   size_t filled = _finder.get_filled_array_count(_images);
   if (filled == 0) {
      return;
   }

   _image_index = (index + static_cast<int>(filled)) % static_cast<int>(filled);
   const std::string &selected_path = _images[static_cast<size_t>(_image_index)];
   load_texture(selected_path);
   _path = selected_path;
}

std::string TextureArray::get_path_at(int index) const
{
   return _images[static_cast<size_t>(index)];
}

int TextureArray::get_image_index() const
{
   return _image_index;
}

} // namespace turtle
