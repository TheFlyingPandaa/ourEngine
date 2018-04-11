#pragma once
#include "Shape.h"

class SkyBoxObject : public Shape
{
public:
	SkyBoxObject();
	std::string toString() const override;

	void Draw() override;
};