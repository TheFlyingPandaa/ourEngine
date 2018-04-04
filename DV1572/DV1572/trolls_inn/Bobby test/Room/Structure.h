#pragma once
#include "../../../ourEngine/interface/Interface.h"

enum WallDirection {
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

	bool	getDir(WallDirection dir) const;
	Tile *	getTile() const;
};

class Tile {
public:
	Tile(int x = 0, int y = 0, int sizeX = 0, int sizeY = 0)
	{
		m_posX = x;
		m_posY = y;
		m_door = false;
		quad.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
		quad.setNormalMap("trolls_inn/Resources/NormalMap.jpg");
		quad.setPos(x, 0, y);
		quad.Rotate(90, 0, 0);
	}
	int		m_posY;
	int		m_posX;
	bool	m_door;
	RectangleShape quad;

	bool	m_walls[4] = { false };
	bool	isWall(WallDirection dir) const { return m_walls[dir]; }
};
