#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "backend_GLSL.h"
#include "base_image.h"
#include "debug.h"

#include <algorithm>
#include <iostream>

Image::Texture::Texture(const std::string &path,
                        const std::string &location_key,
                        float uniform_value)
   : Shape(0, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "TextureArray Default")
   , _image(0)
   , _path(path)
{
   _color = {1.0f, 1.0f, 1.0f, 1.0f};
   _is_color = false;
   if (!location_key.empty()) {
      uniform().set_location_key(location_key);
   }
   uniform().set_uniform_value(uniform_value);

   if (!path.empty()) {
      load_texture(path);
   }
   if (DEBUG_LEVEL == DebugLevel::DETAILED) {
      std::cout << "Texture created with path: " << (path.empty() ? "(empty)" : path) << "\n";
   }
};

Image::Texture::Texture(const vec4 &color,
                        const std::string &location_key,
                        float uniform_value)
   : Texture("", location_key, uniform_value)
{
   upload_color_texture(color);
   _color = color;
   if (DEBUG_LEVEL == DebugLevel::DETAILED) {
      std::cout << "Texture created with solid color: " << color.r << ", " << color.g
                << ", " << color.b << ", " << color.a << "\n";
   }
};

Image::TextureArray::TextureArray(const std::string &folder)
   : Texture("", "scale", 1.0f)
   , _images()
   , _image_index(0)
{
   _image_finder.set_root(folder);

   // Scan all images first
   _image_finder.find_all_images(_images);
   _image_finder.randomize_order(_images);

   // Then load the first (now random) image
   size_t filled_count = _image_finder.get_filled_array_count(_images);
   if (filled_count > 0) {
      load_texture(_images[0]);
      _path = _images[0];
   }

   print_config();
}

void Image::TextureArray::select_texture(int index)
{
   if (_images.empty()) {
      return;
   }

   size_t filled_count = _image_finder.get_filled_array_count(_images);
   _image_index = (index + filled_count) % filled_count;

   if (DEBUG_LEVEL == DebugLevel::DETAILED) {
      std::cout << "Switching to texture: " << _images[_image_index] << "\nAt index: " << index
                << " with image index: " << _image_index << "\n\n";
   }
   load_texture(_images[_image_index]);
}

void Image::Texture::load_texture(const std::string &path)
{
   _is_color = false;
   _path = path;
   _color = {1.0f, 1.0f, 1.0f, 1.0f};

   stbi_set_flip_vertically_on_load(true);

   int width, height, channels;
   unsigned char *pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);
   
   if (DEBUG_LEVEL == DebugLevel::DETAILED) {
      std::cout << path << " loaded with width: " << width << " height: " << height
                << " channels: " << channels << std::endl;
   }
   
   if (!pixels) {
      fprintf(stderr, "Failed to load image: %s\n", path.c_str());
      return;
   }

   GLSL::texture_to_gpu(_image, pixels, width, height);

   stbi_image_free(pixels);
}

void Image::TextureArray::print_config()
{
   if (DEBUG_LEVEL != DebugLevel::BASIC && DEBUG_LEVEL != DebugLevel::DETAILED) {
      return;
   }
   std::cout << "TextureArray size: " << _images.size() << " , index " << _image_index
             << std::endl
             << std::endl;
}

namespace
{
unsigned char color_to_byte(float value)
{
   const float clamped = std::clamp(value, 0.0f, 1.0f);
   return static_cast<unsigned char>(clamped * 255.0f);
}
}

void Image::Texture::upload_color_texture(const vec4 &color)
{
   _is_color = true;
   _color = color;
   _path = "(solid color)";

   unsigned char pixels[4] = {
       color_to_byte(color.r),
       color_to_byte(color.g),
       color_to_byte(color.b),
       color_to_byte(color.a)};

   GLSL::texture_to_gpu(_image, pixels, 1, 1);
}

bool Image::Texture::is_color() const
{
   return _is_color;
}

vec4 Image::Texture::get_color() const
{
   return _color;
}

void Image::Texture::set_solid_color(const vec4 &color)
{
   upload_color_texture(color);
}

void Image::Texture::set_texture_path(const std::string &path)
{
   load_texture(path);
}

void Image::Texture::set_color_value(const vec4 &color)
{
   _color = color;
}
