#include "Rectangle.h"
#include "../../core/Dx.h"
Mesh* RectangleShape::m = nullptr;
void RectangleShape::_buildRectangle()
{
	
	if (m == nullptr)
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
	}
	
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
	if (m)
	{
		delete m;
		m = nullptr;
	}
		
}

void RectangleShape::setDiffuseTexture(const std::string & path)
{
	getMesh()->setDiffuseTexture(path);
}

void RectangleShape::DrawAsHud()
{
	DX::g_HUDQueue.push_back(this);
}
