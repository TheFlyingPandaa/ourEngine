#pragma once
#include <string>
#include "../../ourEngine/interface/Interface.h"

class Room;

enum WallDirection {
	up,
	down,
	left,
	right
};
struct Tile {
	Tile(int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, Room * room = nullptr)
	{
		m_posX = x;
		m_posY = y;
		m_door = false;
		m_room = room;
		quad.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
		quad.setNormalMap("trolls_inn/Resources/NormalMap.jpg");
		quad.setPos(x, 0, y);
		quad.Rotate(90, 0, 0);
	}
	int		m_posY;
	int		m_posX;
	bool	m_door;
	Room*	m_room;
	RectangleShape quad;

	bool	m_walls[4] = { false };	
	bool	isWall(WallDirection dir) const{ return m_walls[dir]; }
};

class Room
{
protected:
	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;
	Tile**	m_tiles;
	bool	m_hasWalls;

	Mesh*	m_wall;

	std::vector<Object3D*> up;
	std::vector<Object3D*> down;
	std::vector<Object3D*> left;
	std::vector<Object3D*> right;

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	virtual ~Room();	

	virtual Tile**		getTiles() const; 
	virtual int			getX() const; 
	virtual int			getY() const;
	virtual int			getSizeX() const; 
	virtual int			getSizeY() const; 

	virtual void		move(int x, int y);
	virtual bool		getHasWalls() const;
	virtual void		CreateWalls(Mesh * mesh);

	virtual bool		Inside(int x, int y); 
	virtual bool		Inside(Tile * t); 

	virtual void Draw() = 0;
	virtual std::string	toString() const = 0;

};

