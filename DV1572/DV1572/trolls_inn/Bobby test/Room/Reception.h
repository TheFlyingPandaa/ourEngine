#pragma once
#include "Room.h"

class Reception : public Room
{
public:
	Reception(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);
	Reception(int posX, int posY, int sizeX, int sizeY, std::vector<Tile*> tiles);

	~Reception();

	// Inherited via Room
	virtual void Draw() override;
	virtual std::string toString() const override;
};

