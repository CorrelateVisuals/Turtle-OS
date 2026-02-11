#pragma once

#include "uniform.h"

#include <array>
#include <string>

namespace turtle
{

class Vertex
{
public:
   Vertex(std::array<float, 3> position = {0.0f, 0.0f, 0.0f},
          std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f},
          std::string name = "Vertex");
   virtual ~Vertex() = default;

   std::array<float, 3> get_position() const;
   std::array<float, 3> get_rotation() const;
   std::string get_name() const;

protected:
   std::array<float, 3> _position;
   const std::array<float, 3> _rotation;
   const std::string _name;
   Uniform _uniform;

   Uniform &uniform();
   const Uniform &uniform() const;
};

class Shape : protected Vertex
{
public:
   using Vertex::uniform;

   Shape(int type = 0, float scale = 1.0f,
         std::array<float, 3> position = {0.0f, 0.0f, 0.0f},
         std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f},
         std::string name = "Shape");
   virtual ~Shape() = default;

   float get_scale() const;
   int get_shape() const;

   const std::array<float, 20> &get_vertices() const;
   const std::array<unsigned int, 6> &get_indices() const;

   void set_quad(float x, float y, float w, float h, float z = 0.0f);

protected:
   std::array<float, 20> _vertices;
   std::array<unsigned int, 6> _indices;
   float _scale;

private:
   int _type;
};

} // namespace turtle
