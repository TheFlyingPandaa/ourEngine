#pragma once
#include "Room.h"
class Kitchen : public Room 
{
public:
	Kitchen(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);
	Kitchen(int posX, int posY, int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles);
	~Kitchen();

	// Inherited via Room
	virtual void Draw() override;
	virtual std::string toString() const override;
};