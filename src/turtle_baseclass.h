#pragma once

#include "image_finder.h"

#include <GLES2/gl2.h>

#include <array>
#include <iostream>
#include <string>

class TurtleBase
{
public:
   enum class DebugLevel { OFF = 0, BASIC = 1, DETAILED = 2 };

   static DebugLevel DEBUG_LEVEL;

   TurtleBase(int argc, char *argv[]);
   ~TurtleBase() = default;

   class Uniform
   {
   public:
      Uniform(GLint l = 0, float v = 0.0f)
         : _shader_location(l)
         , _value(v){};
      ~Uniform() = default;

      float get_uniform_value() const { return _value; };
      void set_uniform_value(float v) { _value = v; };

      GLint get_shader_location() const { return _shader_location; };
      void set_shader_location(GLint l) { _shader_location = l; };

      static Uniform *get_active();
      static void set_active(Uniform *u);

   protected:
      GLint _shader_location;
      float _value;
      static Uniform *_active_uniform;
   };

   class ScaleVertices : public Uniform
   {
   public:
      ScaleVertices(float start, float speed, float final, float ease);
      ~ScaleVertices() = default;

      void run(bool is_on = true);
      void reset_start_scale();

      void set_shader_location(GLint l) { _shader_location = l; }

   private:
      void getSettings();

      float _start_scale;
      float _scale;
      float _zoom_speed;
      float _final_scale;
      float _ease_factor;
   };

   class Geometry
   {
   public:
      class Vertex
      {
      public:
         Vertex(std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
                std::array<float, 3> rot = {0.0f, 0.0f, 0.0f},
                std::string name = "Vertex Default");
         virtual ~Vertex() = default;

         std::array<float, 3> get_position() const { return _position; };
         std::array<float, 3> get_rotation() const { return _rotation; };
         std::string get_name() const { return _name; };

         // protected:
         std::array<float, 3> _position;
         TurtleBase::Uniform _uniform;

         TurtleBase::Uniform &uniform() { return _uniform; }
         const TurtleBase::Uniform &uniform() const { return _uniform; }

      private:
         const std::array<float, 3> _rotation;
         const std::string _name;

         

         void print_config();
      };

      class Shape : protected Vertex
      {
      public:
         using Vertex::uniform;

         Shape(int shape = 0, float scale = 1.0f,
               std::array<float, 3> pos = {0.0f, 0.0f, 0.0f},
               std::array<float, 3> rot = {0.0f, 0.0f, 0.0f},
               std::string name = "Shape Default");
         ~Shape() = default;

         float get_scale() const { return _scale; };
         int get_shape() const { return _shape; };

         const std::array<float, 20> &get_vertices() const { return _vertices; };
         const std::array<unsigned int, 6> &get_indices() const { return _indices; };

         void set_quad(float x, float y, float w, float h, float z = 0.0f);

      protected:
         std::array<float, 20> _vertices = {
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // Top-left (x, y, z, u, v)
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f  // Top-right
         };

         std::array<unsigned int, 6> _indices = {0, 1, 2, 2, 3, 0};
         float _scale;

      private:
         int _shape;

         void print_config();
      };
   };

   class Image
   {
   public:
      class Texture : public Geometry::Shape
      {
      public:
         using Geometry::Shape::uniform;

         Texture(const std::string &path);
         Texture()
            : Texture("")
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

protected:
private:
};