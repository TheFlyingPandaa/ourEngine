#pragma once
#include <iostream>
#include <vector>
#include "RoomCtrl.h"
#include <algorithm>
#include <memory>

struct Node
{
	Tile* tile;
	std::shared_ptr<Node> parent;
	float fCost, gCost, hCost;
	Node(Tile* tile, Node* parent, float gCost, float hCost)
		:tile(tile), parent(parent), gCost(gCost), hCost(hCost) {
		fCost = gCost + hCost;
	}
	Node(Tile* tile, std::shared_ptr<Node> parent, float gCost, float hCost)
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

	Mesh*				m_tileMesh;
	Mesh*				m_gridMesh;
	Object3D			m_wholeGrid;

	std::vector<Tile*> m_tiles;

	bool _findInVec(std::vector<Node*>& list, Node* node) const;
	bool _findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const;
	
	int _index(int x, int y) const;

public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8, Mesh * mesh = nullptr);
	~Grid();

	std::vector<Tile *> getGrid() const;	
	std::vector<Tile*> extractTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size);

	void	Draw();

	void	PickTiles(Shape* selectionTile = nullptr);
	bool	CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end);

	void	ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end);

	void	DrawString();

	float getDistance(Tile* t1, Tile* t2);
	std::vector<std::shared_ptr<Node>> findPathHighLevel(Tile* startTile, Tile* endTile);
	std::vector<std::shared_ptr<Node>> findPath(Tile* startTile, Tile* endTile, bool outside);

};

