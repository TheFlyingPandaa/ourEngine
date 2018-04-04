#include "Rectangle.h"
#include "../../core/Dx.h"

void RectangleShape::_buildRectangle()
{
	m_hud = false;
	
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
	this->setScale(w / 4500, h / 4500, 1.0f);

	setVertexShader(DX::g_3DVertexShader);
	setPixelShader(DX::g_3DPixelShader);
	setHullShader(DX::g_standardHullShader);
	setDomainShader(DX::g_standardDomainShader);
}

RectangleShape::~RectangleShape()
{
	delete m;
}

void RectangleShape::setWidth(float w)
{
	m_width = w;
	this->setScale(w / 4500, m_height / 4500, 1.0f);
}

float RectangleShape::getWidth() const
{
	return m_width;
}

void RectangleShape::setHeight(float h)
{
	m_height = h;
	this->setScale(m_width / 4500, h / 4500, 1.0f);
}

float RectangleShape::getHeight() const
{
	return m_height;
}

void RectangleShape::setDiffuseTexture(const std::string & path)
{
	getMesh()->setDiffuseTexture(path);
}

void RectangleShape::DrawAsHud()
{
	m_hud = true;
	DX::g_HUDQueue.push_back(this);
}

bool RectangleShape::isHud() const
{
	return m_hud;
}

std::string RectangleShape::toString() const
{

	return Shape::toString() + "\n";
}
