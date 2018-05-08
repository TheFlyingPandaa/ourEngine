#include "Text.h"
#include "../core/Dx.h"
#include "Input.h"

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
	m_rt = RelativeTo::BL;
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
	DirectX::XMINT2 s = Input::getWindowSize();
	DirectX::XMFLOAT2 pos(x, y);

	switch (m_rt)
	{
	case Text::BR:
		pos.x = static_cast<float>(s.x) - pos.x;
		break;
	case Text::TR:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case Text::TL:
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case Text::C:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		break;
	case Text::BC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		break;
	case Text::TC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case Text::LC:
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		break;
	case Text::RC:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		break;
	}


	m_textPosition = XMVectorSet(pos.x, pos.y, 0.0f, 1.0f);
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

void Text::setRelative(Text::RelativeTo rt)
{
	m_rt = rt;
}

Text::RelativeTo Text::getRelative() const
{
	return m_rt;
}

void Text::Draw()
{
	DX::g_textQueue.push_back(this);
}

DirectX::XMVECTOR Text::getStringSize(Text * t)
{
	std::string str = t->getTextString();
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const wchar_t* text = widestr.c_str();

	return DX::g_fonts[t->getFontType()]->MeasureString(text);
}
