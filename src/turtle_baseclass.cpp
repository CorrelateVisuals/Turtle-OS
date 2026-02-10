#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "backend_GLSL.h"
#include "turtle_baseclass.h"

#include <iostream>
#include <string>

TurtleBase::DebugLevel TurtleBase::DEBUG_LEVEL = TurtleBase::DebugLevel::BASIC;

TurtleBase::TurtleBase(int argc, char *argv[])
{
   for (int i = 1; i < argc; ++i) {
      std::string argument = argv[i];
      if (argument == "turtle") {
         if (DEBUG_LEVEL != DebugLevel::OFF) {
            std::cout << "TurtleBase found!\n" << std::endl;
         }
         return;
      }
   }
}

TurtleBase::Geometry::Vertex::Vertex(std::array<float, 3> pos, std::array<float, 3> rot,
                                     std::string name)
   : _position(pos)
   , _rotation(rot)
   , _name(name)
{
   print_config();
}

void TurtleBase::Geometry::Vertex::print_config()
{
   if (DEBUG_LEVEL != DebugLevel::BASIC && DEBUG_LEVEL != DebugLevel::DETAILED) {
      return;
   }
   std::cout << _name << " created at position and rotation: " << std::endl
             << "    "
             << "x: " << _position[0] << " y: " << _position[1] << " z: " << _position[2]
             << std::endl
             << "    "
             << "x: " << _rotation[0] << " y: " << _rotation[1] << " z: " << _rotation[2]
             << std::endl
             << std::endl;
}

TurtleBase::Geometry::Shape::Shape(int shape, float scale, std::array<float, 3> pos,
                                   std::array<float, 3> rot, std::string name)
   : Vertex(pos, rot, name)
   , _scale(scale)
   , _shape(shape)
{
   print_config();
}

void TurtleBase::Geometry::Shape::print_config()
{
   if (DEBUG_LEVEL != DebugLevel::BASIC && DEBUG_LEVEL != DebugLevel::DETAILED) {
      return;
   }
   std::cout << "Shape number type: " << _shape << " size of " << _scale << std::endl
             << std::endl;
}

void TurtleBase::Image::TextureArray::print_config()
{
   if (DEBUG_LEVEL != DebugLevel::BASIC && DEBUG_LEVEL != DebugLevel::DETAILED) {
      return;
   }
   std::cout << "TextureArray size: " << _images.size() << " , index " << _image_index
             << std::endl
             << std::endl;
}

// TurtleBase::Image::Texture::Texture()
//    : Shape(0, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "TextureArray Default")
//    , _path("") 
// {
//    std::cout << "Default Texture created with no path\n";
// }

TurtleBase::Image::Texture::Texture(const std::string &path)
   : Shape(0, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "TextureArray Default")
   , _image(0)
   , _path(path)
{
   if (!path.empty()) {
      load_texture(path);
   }
   if (DEBUG_LEVEL == DebugLevel::DETAILED) {
      std::cout << "Texture created with path: " << (path.empty() ? "(empty)" : path) << "\n";
   }
};


TurtleBase::Image::TextureArray::TextureArray(const std::string &folder)
   : Texture(get_first_image_path(folder)), _image_index(0)
   , _images()
{
   _image_finder.set_root(folder);
   _image_finder.find_all_images(_images);
   _image_finder.randomize_order(_images);

   select_texture(_image_index);
   print_config();
}

std::string TurtleBase::Image::TextureArray::get_first_image_path(const std::string &folder)
{
   std::array<std::string, TEXTURE_ARRAY_SIZE> temp_images;
   ImageFinder temp_finder;
   
   temp_finder.set_root(folder);
   temp_finder.find_all_images(temp_images);
   
   size_t filled_count = temp_finder.get_filled_array_count(temp_images);
   if (filled_count > 0) {
      return temp_images[0];
   }
   return "";
}

void TurtleBase::Image::TextureArray::select_texture(int index)
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

void TurtleBase::Image::Texture::load_texture(const std::string &path)
{
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

TurtleBase::ScaleVertices::ScaleVertices(float start, float speed, float final, float ease)
   : Uniform(0, start)
   , _start_scale{start}
   , _scale(start)
   , _zoom_speed(speed)
   , _final_scale(final + speed)
   , _ease_factor(ease)
{
   if (DEBUG_LEVEL == DebugLevel::BASIC) {
      std::cout << "Initializing Zoom." << std::endl;
   }
   set_uniform_value(_scale);
   getSettings();
}

void TurtleBase::ScaleVertices::run(bool is_on)
{
   if (is_on) {
      if (_scale > _final_scale) {
         _scale -= _zoom_speed * (_scale - _final_scale) *
                   _ease_factor; // Adjust the factor as needed
      } else {
         _scale = _final_scale;
      }

      set_uniform_value(_scale);
   }
}

void TurtleBase::ScaleVertices::reset_start_scale()
{
   _scale = _start_scale;
   set_uniform_value(_start_scale);
}

void TurtleBase::ScaleVertices::getSettings()
{
   if (DEBUG_LEVEL == DebugLevel::BASIC) {
      std::cout << "Zoom _scale_location: " << _shader_location
                << ",  _scale: " << get_uniform_value() << ", _zoom_speed: " << _zoom_speed
                << ", _final_scale:" << _final_scale << std::endl
                << std::endl;
   }
}
