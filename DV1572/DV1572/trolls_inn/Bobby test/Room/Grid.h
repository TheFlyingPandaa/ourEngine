#pragma once
#include <iostream>
#include <vector>
#include "RoomCtrl.h"


class Grid
{
private:

	int					m_posX, m_posY;
	int					m_sizeX, m_sizeY;
	bool				m_walls[4] = { false };
	//Tile**				m_tiles;
	Mesh*				m_tileMesh;
	RoomCtrl			m_roomCtrl;
	std::vector<std::vector<Tile*>> m_tiles;


	bool				_intersect(Room * room, bool close = false, Room ** otherRef = nullptr, bool getRef = false);
	
public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8, Mesh * mesh = nullptr);
	~Grid();

	Tile**	getGrid() const;
	
	void	AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, bool force);
	void	Update(Camera * cam);
	void	Draw();

	void	PickTiles();

	void	DrawString();

	void	CreateWalls(Mesh * mesh = nullptr);
};

