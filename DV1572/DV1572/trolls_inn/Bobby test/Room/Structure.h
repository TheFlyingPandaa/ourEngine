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

	bool				isDoor = false;
	
public:
	Wall(Tile* tile, Mesh * mesh = nullptr);
	~Wall();

	void	setIsDoor(bool value);
	bool	getIsDoor() const;

	void	Draw();

	bool	getIsInner() const;
	void	setIsInner(bool isInner);

	void	setPosition(DirectX::XMFLOAT2 position);
	void	setRotation(DirectX::XMFLOAT3 rotation);
	void	setScale(float x, float y, float z);

	DirectX::XMINT2	getPosition() const;

	void	setMesh(Mesh * mesh);

	bool	getDir(Direction dir) const;
	Tile *	getTile() const;
	bool	operator==(const Wall& other);
};

class Tile {
public:
	Tile(int sizeX = 0, int sizeY = 0, Mesh * mesh = nullptr)
	{
		m_door = false;
		
		quad.setMesh(mesh);
		quad.setPos(static_cast<float>(sizeX), 0.0f, static_cast<float>(sizeY));
		quad.setRotation(90.0f, 0.0f, 0.0f);		
		m_isWalkeble = true;
	}
	void	setAdjacent(Tile* tile, Direction dir);
	Room*	getRoom() const;
	void	setRoom(Room * room);
	void	setInside(bool value);
	void	setIsWalkeble(bool value);

	int		getPosX() const;
	int		getPosY() const;

	

	Tile*	getAdjacent(Direction dir);

	bool	getIsWalkeble() const;

	bool	isWall(Direction dir) const { return m_walls[dir]; }
	void	setTileWalls(Direction dir, Wall* value);
	Wall*	getTileWalls(Direction dir) const;

	void	setWallSpotPopulated(Direction dir, bool value);
	bool	getWallSpotPopulated(Direction dir) const;

	Object3D&	getQuad();
	void	setMesh(Mesh * mesh);
private:
	int		m_posY;
	int		m_posX;
	bool	m_door;
	Object3D quad;
	bool isWalkbale() const;

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

	//Tile*	adjacent[4] = { nullptr };
	Wall*	m_w[4] = { nullptr };
	
	bool	m_walls[4] = { false };
	XMFLOAT2 getPosition() const;
	bool operator==(const Tile& other) const;
};
