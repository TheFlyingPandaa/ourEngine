#pragma once
#include <vector>
#include <iostream>
#include "Room.h"
#include "Structure.h"
#include "Kitchen.h"

enum RoomType {
	kitchen
};

const float WALLOFFSET = 0.5f;

class RoomCtrl
{
private:
	std::vector<Room*>	m_rooms;
	std::vector<Wall*>	m_walls;
	

	Mesh*				m_wall;
public:
	RoomCtrl();
	~RoomCtrl();

	void setMesh(Mesh * mesh);

	void AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<std::vector<Tile*>> tiles, bool force = false);
	void Draw();
	void CreateWalls();
};

