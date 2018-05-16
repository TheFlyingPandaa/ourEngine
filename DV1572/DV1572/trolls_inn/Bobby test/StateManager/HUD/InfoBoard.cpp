#include "OurEngine.h"
#include "InfoBoard.h"

void InfoBoard::_updateScale()
{
	DirectX::XMVECTOR size = Text::getStringSize(&m_texts);
	size *= m_texts.getScale();
	m_background.setWidth(DirectX::XMVectorGetX(size) + m_distFromBoarder * 2);
	m_background.setHeight(DirectX::XMVectorGetY(size) + m_distFromBoarder * 2);
	DirectX::XMFLOAT3 p = m_background.getScreenPos();
	m_texts.setPosition(p.x + m_distFromBoarder, p.y + m_distFromBoarder);
}

InfoBoard::InfoBoard()
{
	m_background.setMesh(MeshHandler::getPlainRectangle());
	m_distFromBoarder = 5.0f;
	m_texts.setAllignment(TXT::Left);
	m_texts.setColor(1, 1, 1, 1);
	m_texts.setFontType(TXT::FONT_TYPE::Arial_Black);
	m_texts.setRelative(Text::RelativeTo::BL);
	m_texts.setPosition(0, 0);
	m_texts.setRotation(0);
	m_texts.setScale(0.5f);
	m_texts.setTextString("");
}

InfoBoard::~InfoBoard()
{

}

void InfoBoard::setString(const std::string & text)
{
	m_texts.setTextString(text);
	_updateScale();
}

void InfoBoard::setString(const std::string & text, TXT::FONT_TYPE font)
{
	m_texts.setTextString(text);
	m_texts.setFontType(font);


	_updateScale();
}

void InfoBoard::setTextColor(float r, float g, float b, float a)
{
	m_texts.setColor(r, g, b, a);
}

void InfoBoard::setTextSize(float s)
{
	m_texts.setScale(s);
	_updateScale();
}

void InfoBoard::setPosition(float x, float y)
{
	m_background.setScreenPos(x, y);
	m_texts.setPosition(x + m_distFromBoarder, y + m_distFromBoarder);
}

void InfoBoard::setPosition(float x, float y, RectangleShape::RelativeTo allignment)
{
	m_background.setRelative(allignment);
	m_texts.setRelative(static_cast<Text::RelativeTo>(allignment));

	setPosition(x, y);
}

void InfoBoard::setPosition(float x, float y, Text::RelativeTo allignment)
{
	m_background.setRelative(static_cast<RectangleShape::RelativeTo>(allignment));
	m_texts.setRelative(allignment);


	setPosition(x, y);
}

void InfoBoard::Draw()
{
	m_background.DrawAsHud();
	m_texts.Draw();
}
