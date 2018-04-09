#include "Rectangle.h"
#include "../../core/Dx.h"

void RectangleShape::_buildRectangle()
{
	m_hud = false;
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
}

void RectangleShape::setScreenPos(float x, float y)
{
	DirectX::XMFLOAT2 pos(x, y);

	this->setScreenPos(pos);
}

void RectangleShape::setScreenPos(DirectX::XMFLOAT2 pos)
{
	DirectX::XMFLOAT3 start(-0.0736f, -0.0414f, 0.0f);
	start.x += (pos.x / 8625.0f);
	start.y += (pos.y / 8625.0f);

	Shape::setPos(start);




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

void RectangleShape::setNormalMap(const std::string & path)
{
	getMesh()->setNormalTexture(path);
}

void RectangleShape::DrawAsHud()
{
	m_hud = true;
	DX::submitToInstance(this, DX::g_instanceGroupsHUD);
}

bool RectangleShape::isHud() const
{
	return m_hud;
}

std::string RectangleShape::toString() const
{

	return Shape::toString() + "\n";
}
