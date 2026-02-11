#ifndef TURTLE_OS_MESH_H
#define TURTLE_OS_MESH_H

#include "shader.h"

class Mesh {
public:
    Mesh();
    ~Mesh();

    void init();
    void draw(const Shader &shader) const;

private:
    unsigned int _vbo;
};

#endif // TURTLE_OS_MESH_H
