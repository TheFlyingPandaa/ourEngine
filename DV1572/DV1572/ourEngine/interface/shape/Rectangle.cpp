#include "Rectangle.h"
RectangleShape::RectangleShape(float w, float h)
{
	m = new Mesh;
	std::vector<VERTEX> vertices;
	m_height = h;
	m_width = w;

	VERTEX v = {

		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		-0.5f, 0.5f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, -0.5f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);
	m->LoadModel(vertices);
	setMesh(m);
}

RectangleShape::~RectangleShape()
{
	delete m;
}
