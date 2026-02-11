#pragma once

#include "base_geometry.h"
#include "image_finder.h"

#include <GLES2/gl2.h>

#include <array>
#include <string>

class Image
{
public:
   class Texture : public Geometry::Shape
   {
   public:
      using Geometry::Shape::uniform;

      Texture(const std::string &path, const std::string &location_key = "scale",
               float uniform_value = 1.0f);
      Texture()
         : Texture("", "scale", 1.0f)
      {
      }
      ~Texture() = default;

      std::string get_path() const { return _path; }
      GLuint get_image() const { return _image; }

   protected:
      void load_texture(const std::string &path);

   private:
      GLuint _image;

   protected:
      std::string _path;
   };

   class TextureArray : public Texture
   {
   public:
      TextureArray(const std::string &folder);
      ~TextureArray() = default;

      void select_texture(int index);
      std::string get_path_at(int index) const { return _images[index]; }
      int get_image_index() const { return _image_index; }

   private:
      ImageFinder _image_finder;
      std::array<std::string, TEXTURE_ARRAY_SIZE> _images;
      int _image_index;

      void print_config();
   };
};
