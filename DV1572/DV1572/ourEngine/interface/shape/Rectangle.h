#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
private:
	float m_width, m_height;
	static Mesh* m;
	bool m_hud;

private:
	void _buildRectangle();

public:
	RectangleShape(float w = 10.0f, float h = 10.0f);
	~RectangleShape();
	void setScreenPos(float x, float y);
	void setScreenPos(DirectX::XMFLOAT2 pos);


	void setWidth(float w);
	float getWidth() const;
	void setHeight(float h);
	float getHeight() const;
	void setDiffuseTexture(const std::string &path);
	void setNormalMap(const std::string &path);
	void DrawAsHud();
	//Needs to be Drawn as hud once for this to be true
	bool isHud() const;
	std::string toString() const override;

};