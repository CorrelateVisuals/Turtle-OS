#pragma once

#include "geometry.h"
#include "image_finder.h"

#include <GLES2/gl2.h>

#include <array>
#include <string>

namespace turtle
{

class Texture : public Shape
{
public:
   using Shape::uniform;

   Texture(const std::string &path, const std::string &location_key = "scale",
           float uniform_value = 1.0f);
   Texture();
   virtual ~Texture() = default;

   std::string get_path() const;
   GLuint get_image() const;

protected:
   void load_texture(const std::string &path);
   void upload_pixels(const unsigned char *pixels, int width, int height);

private:
   GLuint _image;

protected:
   std::string _path;
};

class TextureArray : public Texture
{
public:
   explicit TextureArray(const std::string &folder);
   ~TextureArray() override = default;

   void select_texture(int index);
   std::string get_path_at(int index) const;
   int get_image_index() const;

private:
   ImageFinder _finder;
   std::array<std::string, TEXTURE_ARRAY_SIZE> _images;
   int _image_index;
};

} // namespace turtle
