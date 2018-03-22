#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
private:
	float m_width, m_height;
	Mesh* m;

public:
	RectangleShape(float w = 1.0f, float h = 1.0f);
	~RectangleShape();
	void setWidth(float w);
	float getWidth() const;
	void setHeight(float h);
	float getHeight() const;
};