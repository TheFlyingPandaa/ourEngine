#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
public:
	enum RelativeTo
	{
		BL,		// Bottom Left
		BR,		// Bottom Right
		TR,		// Top Right
		TL,		// Top Left
		C,		// Center of the screen
		BC,		// Bottom Center
		TC,		// Top Center
		LC,		// Left Center
		RC		// Right Center
	};

private:
	float m_width, m_height;
	bool m_hud;
	int m_index;
	RelativeTo m_rt;

private:
	void _buildRectangle();

public:
	RectangleShape(float w = 10.0f, float h = 10.0f);
	~RectangleShape();
	void setScreenPos(float x, float y, float depth = 0.0f);
	void setScreenPos(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 getScreenPos() const;

	int getIndex() const;
	void setIndex(int index);
	void setWidth(float w);
	float getWidth() const;
	void setHeight(float h);
	float getHeight() const;
	void setDiffuseTexture(const std::string &path);
	void setNormalMap(const std::string &path);
	void DrawAsHud();
	void setRelative(RectangleShape::RelativeTo rt);
	//Needs to be Drawn as hud once for this to be true
	bool isHud() const;
	std::string toString() const override;

};