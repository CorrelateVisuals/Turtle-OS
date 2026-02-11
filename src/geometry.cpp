#include "geometry.h"

#include <iostream>

namespace turtle
{

Vertex::Vertex(std::array<float, 3> position, std::array<float, 3> rotation, std::string name)
   : _position(position)
   , _rotation(rotation)
   , _name(std::move(name))
   , _uniform(-1, 0.0f)
{
}

std::array<float, 3> Vertex::get_position() const
{
   return _position;
}

std::array<float, 3> Vertex::get_rotation() const
{
   return _rotation;
}

std::string Vertex::get_name() const
{
   return _name;
}

Uniform &Vertex::uniform()
{
   return _uniform;
}

const Uniform &Vertex::uniform() const
{
   return _uniform;
}

Shape::Shape(int type, float scale, std::array<float, 3> position,
             std::array<float, 3> rotation, std::string name)
   : Vertex(position, rotation, std::move(name))
   , _vertices({
         -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // Top-left
         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
         0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f  // Top-right
      })
   , _indices({0, 1, 2, 2, 3, 0})
   , _scale(scale)
   , _type(type)
{
}

float Shape::get_scale() const
{
   return _scale;
}

int Shape::get_shape() const
{
   return _type;
}

const std::array<float, 20> &Shape::get_vertices() const
{
   return _vertices;
}

const std::array<unsigned int, 6> &Shape::get_indices() const
{
   return _indices;
}

void Shape::set_quad(float x, float y, float w, float h, float z)
{
   _position = {x, y, z};
   _scale = w;
   _vertices = {
      x,     y,     z, 0.0f, 1.0f, // Top-left
      x,     y - h, z, 0.0f, 0.0f, // Bottom-left
      x + w, y - h, z, 1.0f, 0.0f, // Bottom-right
      x + w, y,     z, 1.0f, 1.0f  // Top-right
   };
}

} // namespace turtle
