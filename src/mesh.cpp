#include "mesh.h"

#include <GLES2/gl2.h>

Mesh::Mesh() : BaseMesh(), _vbo(0) {}

Mesh::~Mesh() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
    }
}

void Mesh::init() {
    set_quad();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertex_count(), data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(const Shader &shader) const {
    int pos_location = shader.get_attrib_location("pos");
    int uv_location = shader.get_attrib_location("uv");

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    if (pos_location >= 0) {
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float), reinterpret_cast<void *>(0));
    }

    if (uv_location >= 0) {
        glEnableVertexAttribArray(uv_location);
        glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float),
                              reinterpret_cast<void *>(2 * sizeof(float)));
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count());

    if (pos_location >= 0) {
        glDisableVertexAttribArray(pos_location);
    }
    if (uv_location >= 0) {
        glDisableVertexAttribArray(uv_location);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
