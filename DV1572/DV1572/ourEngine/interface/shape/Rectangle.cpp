#include "Rectangle.h"
#include "../../core/Dx.h"
#include "../Input.h"

void RectangleShape::_buildRectangle()
{
	m_hud = false;
	m_index = -1;
}

RectangleShape::RectangleShape(float w, float h)
{
	DirectX::XMINT2 s = Input::getWindowSize();
	m_rt = RelativeTo::BL;
	m_height = (h / s.y) * 4;
	m_width = (w / s.x) * 4;

	_buildRectangle();
	
	
	this->setScale(m_width, m_height, 1.0f);

	setVertexShader(DX::g_3DVertexShader);
	setPixelShader(DX::g_3DPixelShader);
	setHullShader(DX::g_standardHullShader);
	setDomainShader(DX::g_standardDomainShader);
}

RectangleShape::~RectangleShape()
{
}

void RectangleShape::setScreenPos(float x, float y, float depth)
{
	DirectX::XMFLOAT3 pos(x, y, depth);

	this->setScreenPos(pos);
}

void RectangleShape::setScreenPos(DirectX::XMFLOAT3 pos)
{
	DirectX::XMINT2 s = Input::getWindowSize();
	DirectX::XMFLOAT3 scl = Shape::getScale();
	DirectX::XMFLOAT3 scrnPos;

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	switch (m_rt)
	{
	case RectangleShape::BR:
		pos.x = static_cast<float>(s.x) - pos.x;
		offsetX = - scl.x;
		break;
	case RectangleShape::TR:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetX = - scl.x;
		offsetY = - scl.y;
		break;
	case RectangleShape::TL:
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetY = - scl.y;
		break;
	case RectangleShape::C:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetX = -(scl.x / 2.0f);
		offsetY = -(scl.y / 2.0f);
		break;
	case RectangleShape::BC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		offsetX = -(scl.x / 2.0f);
		break;
	case RectangleShape::TC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetX = -(scl.x / 2.0f);
		offsetY = -scl.y;
		break;
	case RectangleShape::LC:
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetY = -(scl.y / 2.0f);
		break;
	case RectangleShape::RC:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetX = -scl.x;
		offsetY = -(scl.y / 2.0f);
		break;
	}


	scrnPos.x = (2.0f * ((pos.x) / s.x) - 1.0f) + (offsetX / 2);
	scrnPos.y = (2.0f * ((pos.y) / s.y) - 1.0f) + (offsetY / 2);
	scrnPos.z = pos.z;

	Shape::setPos(scrnPos);
	//Shape::setPos(pos);
}

DirectX::XMFLOAT3 RectangleShape::getScreenPos() const
{
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(getPosition());

	DirectX::XMINT2 s = Input::getWindowSize();
	pos.x = 0.5f * (pos.x * s.x) + 1.0f;
	pos.y = 0.5f * (pos.y * s.y) + 1.0f;

	return pos;
}

int RectangleShape::getIndex() const
{
	return m_index;
}

void RectangleShape::setIndex(int index)
{
	m_index = index;
}



void RectangleShape::setWidth(float w)
{
	DirectX::XMINT2 s = Input::getWindowSize();
	m_width = (w / s.x) * 4;

	this->setScale(m_width, m_height, 1.0f);
}

void RectangleShape::setHeight(float h)
{
	DirectX::XMINT2 s = Input::getWindowSize();
	m_height = (h / s.y) * 4;

	this->setScale(m_width, m_height, 1.0f);
}
float RectangleShape::getWidth() const
{
	return ((m_width / 4) * Input::getWindowSize().x) ;
}


float RectangleShape::getHeight() const
{
	return ((m_height / 4) * Input::getWindowSize().y);
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

void RectangleShape::setRelative(RectangleShape::RelativeTo rt)
{
	m_rt = rt;
}

bool RectangleShape::isHud() const
{
	return m_hud;
}

std::string RectangleShape::toString() const
{

	return Shape::toString() + "\n";
}
