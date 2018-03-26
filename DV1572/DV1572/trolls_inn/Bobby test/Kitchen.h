#pragma once
#include "Room.h"
class Kitchen : public Room 
{
public:
	Kitchen(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0);
	~Kitchen();

	// Inherited via Room
	virtual std::string toString() const override;
};