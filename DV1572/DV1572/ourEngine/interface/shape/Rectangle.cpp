#include "Rectangle.h"
#include "../../core/Dx.h"

void RectangleShape::_buildRectangle()
{
	m = new Mesh;
	std::vector<VERTEX> vertices;

	VERTEX v = {

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		0.0f, 0.5f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {
		0.5f, 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);

	v = {

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	vertices.push_back(v);
	m->LoadModel(vertices);
	setMesh(m);
}

RectangleShape::RectangleShape(float w, float h)
{
	m_height = h;
	m_width = w;
	
	_buildRectangle();

	setVertexShader(DX::g_3DVertexShader);
	setPixelShader(DX::g_3DPixelShader);
	setHullShader(DX::g_standardHullShader);
	setDomainShader(DX::g_standardDomainShader);
}

RectangleShape::~RectangleShape()
{
	delete m;
}

void RectangleShape::setDiffuseTexture(const std::string & path)
{
	getMesh()->setDiffuseTexture(path);
}
