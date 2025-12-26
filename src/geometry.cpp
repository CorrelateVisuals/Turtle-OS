#include "geometry.h"

Geometry::Vertex::Vertex(std::array<float, 3> pos, std::array<float, 3> rot, std::string name)
    : _position(pos), _rotation(rot), _name(name)
{
    print_config();
}

void Geometry::Vertex::print_config()
{
    std::cout << _name << " created at position and rotation: " << std::endl
              << "    " << "x: " << _position[0] << " y: " << _position[1]
              << " z: " << _position[2] << std::endl
              << "    " << "x: " << _rotation[0] << " y: " << _rotation[1]
              << " z: " << _rotation[2] << std::endl
              << std::endl;
}

Geometry::Shape::Shape(int shape, float scale, std::array<float, 3> pos, std::array<float, 3> rot, std::string name)
    : Vertex(pos, rot, name), _scale(scale), _shape(shape)
{
    print_config();
}

void Geometry::Shape::print_config()
{
    std::cout << "Shape number type: " << _shape << " size of " << _scale << std::endl
              << std::endl;
}