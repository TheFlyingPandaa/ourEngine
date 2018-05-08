#pragma once
#include <iostream>
#include <vector>
#include "RoomCtrl.h"
#include <algorithm>
#include "Node.h"
#include "../Character.h"

class Grid
{
private:

	int					m_posX, m_posY;
	int					m_sizeX, m_sizeY;

	Mesh*				m_tileMesh;
	Mesh*				m_gridMesh;
	Mesh*				m_catWalkMesh;
	Object3D			m_wholeGrid;

	std::vector<Tile*> m_tiles;
	Object3D			m_catWalkTile;

	bool _findInVec(std::vector<Node*>& list, Node* node) const;
	bool _findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const;
	
	int _index(int x, int y) const;

public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8, Mesh * mesh = nullptr);
	~Grid();

	std::vector<Tile *> getGrid() const;	
	std::vector<Tile*> extractTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	void insertTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size, std::vector<Tile*>& tiles);

	Tile* getTile(int x, int y);

	void	Draw();

	void	PickTiles(Shape* selectionTile = nullptr);
	bool	CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end);
	bool	MarkAllTilesRed(DirectX::XMINT2 start, DirectX::XMINT2 end);

	void	ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end);

	void	DrawString();

	void generatePath(Character& character, RoomType targetRoom);
	float getDistance(Tile* t1, Tile* t2);
	std::vector<std::shared_ptr<Node>> findPath(Tile* startTile, Tile* endTile);

};

