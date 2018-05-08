#pragma once

#include "Shape.h"

class Billboard : public Shape
{
private:
	int m_spriteIndex;
public:
	Billboard();
	int getSpriteIndex() const;
	void setSpriteIndex(int index);

	std::string toString() const override;

};

