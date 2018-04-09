#pragma once
#include "../../../ourEngine/interface/Interface.h"

enum Direction {
	up,
	down,
	left,
	right
};
//deferred context

class Tile;

class Wall
{
private:
	bool				m_wallDir[4] = { false };
	bool				m_innerWall;
	Object3D			m_wall;
	Tile*				m_tile;

	DirectX::XMINT2		position;
	
public:
	Wall(Tile* tile, Mesh * mesh = nullptr);
	~Wall();

	void	Draw();

	bool	getIsInner() const;
	void	setIsInner(bool isInner);

	void	setPosition(DirectX::XMFLOAT2 position);
	void	setRotation(DirectX::XMFLOAT3 rotation);
	void	setScale(float x, float y, float z);

	bool	getDir(Direction dir) const;
	Tile *	getTile() const;
};

class Tile {
public:
	Tile(int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, Mesh * mesh = nullptr)
	{
		m_posX = x;
		m_posY = y;
		m_door = false;
		
		quad.setMesh(mesh);
		quad.setPos(static_cast<float>(x), 0.0f, static_cast<float>(y));
		quad.Rotate(90.0f, 0.0f, 0.0f);		
		m_walkable = true;
	}
	void	setAdjacent(Tile* tile, Direction dir);
	Tile*	getAdjacent(Direction dir);
	int		m_posY;
	int		m_posX;
	bool	m_door;
	Object3D quad;
	bool m_walkable;
	bool isWalkbale() const
	{
		return m_walkable;
	}

	Tile*	adjacent[4] = { nullptr };

	Wall*	m_w[4] = { nullptr };

	bool	m_walls[4] = { false };
	bool	isWall(Direction dir) const { return m_walls[dir]; }
};
