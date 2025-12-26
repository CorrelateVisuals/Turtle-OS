#include "geometry.h"

Geometry::Vertex::Vertex(std::array<float, 3> pos, std::array<float, 3> rot, std::string name)
    : _position(pos), _rotation(rot), _name(name)
{
    print_config();
};
