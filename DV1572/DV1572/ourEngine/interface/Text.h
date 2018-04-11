#pragma once
#include <DirectXMath.h>
#include <string>


namespace TXT
{
	enum FONT_TYPE
	{
		Consolas,
		Constantina,
		Constantina_Big,
		Arial_Black,
	};
	enum ALLIGN
	{
		Left,
		Center,
		Right
	};
}

class Text
{
private:
	TXT::FONT_TYPE			m_type;
	DirectX::XMVECTOR		m_textPosition;
	DirectX::XMVECTOR		m_scale;
	float					m_rotation;
	DirectX::XMVECTORF32	m_color;
	std::string				m_text;
	TXT::ALLIGN				m_allignment;
public:
	Text();
	TXT::FONT_TYPE			getFontType() const;
	DirectX::XMVECTOR		getPosition() const;
	DirectX::XMVECTOR		getScale() const;
	float					getRotation() const;
	DirectX::XMVECTORF32	getColor() const;
	std::string				getTextString() const;
	TXT::ALLIGN				getAllignment() const;

	void setFontType(TXT::FONT_TYPE type);
	void setPosition(float x, float y);
	void setScale(float scl);
	void setRotation(float rot);
	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(DirectX::XMVECTORF32 color);
	void setTextString(const std::string& text);
	void setAllignment(TXT::ALLIGN allignment);

	void Draw();


};