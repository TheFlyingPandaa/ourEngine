#pragma once
#include <iostream>
#include <vector>
#include "Room.h"
#include "Kitchen.h"
#include "Bedroom.h"


class Grid
{
private:
	Tile**	m_tiles;

	std::vector<Room*> rooms;

public:
	Grid();
	~Grid();

	void drawString();

	void addRoom(Room* room);

	Tile** getGrid() const;
};

