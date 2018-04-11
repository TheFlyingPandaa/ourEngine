#pragma once
#include <iostream>
#include <vector>
#include "RoomCtrl.h"
#include <algorithm>
#include "../Objects/ObjectsCtrl.h"

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
	bool operator==(Tile& other) const
	{
		return tile->getQuad().getPosition().x == other.getQuad().getPosition().x && tile->getQuad().getPosition().z == other.getQuad().getPosition().z;
	}
	
	bool operator==(Node& other) const
	{
		return tile->getQuad().getPosition().x == other.tile->getQuad().getPosition().x && tile->getQuad().getPosition().z == other.tile->getQuad().getPosition().z;
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
	bool _findInVec(std::vector<Node*>& list, Node* node) const;

	ObjectsCtrl			m_objCtrl;
public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8, Mesh * mesh = nullptr);
	~Grid();

	std::vector<std::vector<Tile *>> getGrid() const;
	
	void	AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, bool force = false);
	void	Update(Camera * cam);

	void	Draw();

	void	PickTiles();
	bool	CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end);

	void	ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end);

	void	DrawString();

	RoomCtrl&	getRoomCtrl();
	ObjectsCtrl&	getObjectCtrl();

	void	CreateWalls(Mesh * mesh = nullptr);

	float getDistance(Tile* t1, Tile* t2);
	std::vector<Node*> findPath(Tile* startTile, Tile* endTile);
	Tile* getTile(int x, int y) const;

};

