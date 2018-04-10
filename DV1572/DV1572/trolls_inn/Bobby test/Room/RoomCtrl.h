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
	std::vector<std::vector<int>> connections;

	Mesh*				m_wall;
public:
	RoomCtrl();
	~RoomCtrl();

	bool				_intersect(Room * room);
	bool				_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	bool				isPlaceable(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	void setMesh(Mesh * mesh);

	void AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<std::vector<Tile*>> tiles, bool force = false);
	void Update(Camera * cam);
	void Draw();
	void CreateWalls();
};

