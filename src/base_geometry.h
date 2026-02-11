#ifndef TURTLE_OS_BASE_GEOMETRY_H
#define TURTLE_OS_BASE_GEOMETRY_H

#include <array>

struct Vertex2D {
	float x;
	float y;
	float u;
	float v;
};

class BaseMesh {
public:
	BaseMesh();

	void set_quad();
	const float *data() const;
	int vertex_count() const;

private:
	std::array<float, 16> _data;
	int _vertex_count;
};

#endif // TURTLE_OS_BASE_GEOMETRY_H
