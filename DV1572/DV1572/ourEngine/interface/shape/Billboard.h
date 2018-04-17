#pragma once

#include "Shape.h"

class Billboard : public Shape
{
public:
	Billboard();
	std::string toString() const override;

};

