#pragma once
#include "../../../ourEngine/interface/Interface.h"

enum Direction {
	up,
	down,
	left,
	right
};
class Tile;
class Room;

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
	}
	void	setAdjacent(Tile* tile, Direction dir);
	void	setRoom(Room * room);
	void	setInside(bool value);
	void	setIsWalkeble(bool value);

	int		getPosX() const;
	int		getPosY() const;

	Tile*	getAdjacent(Direction dir);

	bool	isWall(Direction dir) const { return m_walls[dir]; }
	void	setTileWalls(Direction dir, Wall* value);
	Wall*	getTileWalls(Direction dir) const;

	void	setWallSpotPopulated(Direction dir, bool value);
	bool	getWallSpotPopulated(Direction dir) const;

	Object3D&	getQuad();
private:
	int		m_posY;
	int		m_posX;
	bool	m_door;
	Object3D quad;

	bool	m_inside = false;
	bool	m_isWalkeble = false;
	Room *	m_room = nullptr;
	Tile*	adjacent[4] = { nullptr };
	Wall*	m_w[4] = { nullptr };
	
	bool	m_walls[4] = { false };
};
