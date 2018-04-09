#pragma once
#include "../../../ourEngine/interface/Interface.h"

enum Direction {
	up,
	down,
	left,
	right,
	upright, // SPECIAL CASES
	upleft,
	downright,
	downleft,
	noneSpecial
};
//deferred context

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
	Tile(int sizeX = 0, int sizeY = 0, Mesh * mesh = nullptr)
	{
		m_door = false;
		
		quad.setMesh(mesh);
		quad.setPos(static_cast<float>(0), 0.0f, static_cast<float>(0));
		quad.Rotate(90.0f, 0.0f, 0.0f);		
		m_walkable = true;
	}
	void	setAdjacent(Tile* tile, Direction dir);
	void	setAdjacent(Tile * tile, int dir);
	Tile*	getAdjacent(Direction dir);
	Tile*	getAdjacent(int dir);
	void	setRoom(Room * room);

	bool	m_door;
	Object3D quad;
	bool m_walkable;
	bool isWalkbale() const
	{
		return m_walkable;
	}

	bool	m_inside = false;
	bool	m_isWalkeble = false;

	Room *	m_room = nullptr;
	/*
	0 = up
	1 = down
	2 = left
	3 = right
	4 = upright
	5 = upleft
	6 = downright
	7 = downleft
	*/
	Tile*	adjacent[8] = { nullptr };

	Wall*	m_w[4] = { nullptr };

	bool	m_walls[4] = { false };
	bool	isWall(Direction dir) const { return m_walls[dir]; }
	XMFLOAT2 getPosition() const;
	bool operator==(const Tile& other) const;
};
