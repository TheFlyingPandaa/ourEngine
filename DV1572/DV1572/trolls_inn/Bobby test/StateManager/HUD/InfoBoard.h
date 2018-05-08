#pragma once
#include "../../../../ourEngine/interface/Interface.h"
#include "../../../Mesh Manager/MeshManager.h"

class InfoBoard
{
private:
	RectangleShape m_background;
	Text m_texts;
	float m_distFromBoarder;

	void _updateScale();
public:
	InfoBoard();
	~InfoBoard();
	void setString(const std::string &text);
	void setString(const std::string &text, TXT::FONT_TYPE font);
	void setTextColor(float r, float g, float b, float a = 1.0f);
	void setTextSize(float s);

	void setPosition(float x, float y);
	void setPosition(float x, float y, RectangleShape::RelativeTo allignment);
	void setPosition(float x, float y, Text::RelativeTo allignment);

	void Draw();
};