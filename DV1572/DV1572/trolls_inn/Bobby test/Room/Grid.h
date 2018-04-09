#pragma once
#include <iostream>
#include <vector>
#include "RoomCtrl.h"
#include <algorithm>

struct Node
{
	Tile* tile;
	Node* parent;
	float fCost, gCost, hCost;
	Node(Tile* tile, Node* parent, float gCost, float hCost)
		:tile(tile), parent(parent), gCost(gCost), hCost(hCost) {
		fCost = gCost + hCost;
	}
	
	bool operator>(const Node& other) const
	{
		return fCost > other.fCost;
	}
	bool operator<(const Node& other) const
	{
		return fCost < other.fCost;
	}
	bool operator==(const Tile& other) const
	{
		return tile->getPosition().x == other.getPosition().x && tile->getPosition().y == other.getPosition().y;
	}
	bool operator==(const Node& other) const
	{
		return tile == other.tile;
	}
};

class Grid
{
private:

	int					m_posX, m_posY;
	int					m_sizeX, m_sizeY;
	bool				m_walls[4] = { false };
	Mesh*				m_tileMesh;
	RoomCtrl			m_roomCtrl;
	std::vector<std::vector<Tile*>> m_tiles;
	
public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8, Mesh * mesh = nullptr);
	~Grid();

	Tile**	getGrid() const;

	std::vector<std::vector<Tile*>> getTiles() const;
	
	void	AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, bool force = false);
	void	Update(Camera * cam);

	void	Draw();

	void	PickTiles();
	bool	CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end);

	void	ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end);

	void	DrawString();

	void	CreateWalls(Mesh * mesh = nullptr);

	float getDistance(const Tile* t1, const Tile* t2) const;
	std::vector<Node*> findPath(Tile* startTile, Tile* endTile) const;
	Tile* getTile(int x, int y) const;

};

