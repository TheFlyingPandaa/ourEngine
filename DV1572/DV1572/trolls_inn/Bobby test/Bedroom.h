#pragma once
#include "Room.h"
class Bedroom : public Room
{
public:
	Bedroom(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0) : Room(posX, posY, sizeX, sizeY){}
	~Bedroom() {
		for (int i = 0; i < m_sizeX; i++)
		{
			delete[] m_tiles[i];
		}
		delete[] m_tiles;
	}
	virtual std::string toString() const override {
		return "B";
	}
};