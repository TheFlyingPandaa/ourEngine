#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
private:
	float m_width, m_height;
	static Mesh* m;

private:
	void _buildRectangle();

public:
	RectangleShape(float w = 1.0f, float h = 1.0f);
	~RectangleShape();
	void setWidth(float w);
	float getWidth() const;
	void setHeight(float h);
	float getHeight() const;
	void setDiffuseTexture(const std::string &path);
	void DrawAsHud();

};