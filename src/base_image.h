#pragma once

#include "base_geometry.h"
#include "image_finder.h"

#include <GLES2/gl2.h>

#include <array>
#include <string>

struct vec4
{
   float r{0.0f};
   float g{0.0f};
   float b{0.0f};
   float a{1.0f};

   vec4() = default;
   vec4(float r_, float g_, float b_, float a_ = 1.0f)
      : r(r_)
      , g(g_)
      , b(b_)
      , a(a_)
   {
   }
};

class Image
{
public:
   class Texture : public Geometry::Shape
   {
   public:
      using Geometry::Shape::uniform;

      Texture(const std::string &path, const std::string &location_key = "scale",
               float uniform_value = 1.0f);
      Texture(const vec4 &color, const std::string &location_key = "scale",
               float uniform_value = 1.0f);
      Texture()
         : Texture("", "scale", 1.0f)
      {
      }
      ~Texture() = default;

      std::string get_path() const { return _path; }
      GLuint get_image() const { return _image; }

      bool is_color() const;
      vec4 get_color() const;

      void set_solid_color(const vec4 &color);
      void set_texture_path(const std::string &path);
      void set_color_value(const vec4 &color);

   protected:
      void load_texture(const std::string &path);
      void upload_color_texture(const vec4 &color);

   private:
      GLuint _image;
      bool _is_color{false};

   protected:
      std::string _path;
      vec4 _color;
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
