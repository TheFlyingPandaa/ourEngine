#pragma once
#include "Shape.h"

class Object3D : public Shape
{
public:
	Object3D();
	std::string toString() const override;
};