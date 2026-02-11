#pragma once

#include "base_shader_access.h"

#include <array>
#include <string>

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
      Uniform _uniform;

      Uniform &uniform() { return _uniform; }
      const Uniform &uniform() const { return _uniform; }

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
