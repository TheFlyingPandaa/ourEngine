#include "Text.h"
#include "../core/Dx.h"

using namespace DirectX;
Text::Text()
{
	m_type = TXT::FONT_TYPE::Consolas;
	m_textPosition = XMVectorSet(0.0f,0.0f,0.0f,1.0f);
	m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);;
	m_rotation = 0.0f;
	m_color = DirectX::Colors::White;
	m_text = "";
	m_allignment = TXT::ALLIGN::Left;
}

TXT::FONT_TYPE Text::getFontType() const
{
	return m_type;
}

DirectX::XMVECTOR Text::getPosition() const
{
	return m_textPosition;
}

DirectX::XMVECTOR Text::getScale() const
{
	return m_scale;
}

float Text::getRotation() const
{
	return m_rotation;
}

DirectX::XMVECTORF32 Text::getColor() const
{
	return m_color;
}

std::string Text::getTextString() const
{
	return m_text;
}

TXT::ALLIGN Text::getAllignment() const
{
	return m_allignment;
}

void Text::setFontType(TXT::FONT_TYPE type)
{
	m_type = type;
}

void Text::setPosition(float x, float y)
{
	m_textPosition = XMVectorSet(x, y, 0.0f, 1.0f);
}

void Text::setScale(float scl)
{
	m_scale = XMVectorSet(scl, scl, scl, 1.0f);
}

void Text::setRotation(float rot)
{
	m_rotation = rot;
}

void Text::setColor(float r, float g, float b, float a)
{
	m_color = XMVECTORF32{ r, g, b, a };
}

void Text::setColor(DirectX::XMVECTORF32 color)
{
	m_color = color;
}

void Text::setTextString(const std::string & text)
{
	m_text = text;
}

void Text::setAllignment(TXT::ALLIGN allignment)
{
	m_allignment = allignment;
}

void Text::Draw()
{
	DX::g_textQueue.push_back(this);
}
