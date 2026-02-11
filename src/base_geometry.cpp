#include "base_geometry.h"

BaseMesh::BaseMesh() : _data{0.0f}, _vertex_count(0) {}

void BaseMesh::set_quad() {
	_data = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f,  -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  0.0f, 1.0f,
		1.0f,  1.0f,  1.0f, 1.0f,
	};
	_vertex_count = 4;
}

const float *BaseMesh::data() const {
	return _data.data();
}

int BaseMesh::vertex_count() const {
	return _vertex_count;
}
