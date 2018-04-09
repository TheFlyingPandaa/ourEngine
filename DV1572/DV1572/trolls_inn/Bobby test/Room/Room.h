#pragma once
#include <string>
#include <vector>
#include "../../../ourEngine/interface/Interface.h"
#include "Structure.h"

class Room
{
protected:
	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;

	std::vector<std::vector<Tile*>> m_tiles;

	bool	m_culledWalls[4] = { false };

	//TODO
	std::vector<Wall*> up;
	std::vector<Wall*> down;
	std::vector<Wall*> left;
	std::vector<Wall*> right;

	std::vector<Room*> adjasent;
	
public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	Room(int posX, int posY , int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles);

	virtual ~Room();	

	virtual std::vector<std::vector<Tile*>>		getTiles() const; 
	virtual Tile*								getTiles(int x, int y) const;

	virtual void		setTile(Mesh * mesh);
	
	virtual int			getX() const; 
	virtual int			getY() const;
	virtual int			getSizeX() const; 
	virtual int			getSizeY() const; 
	virtual void		setWalls(std::vector<Wall*> walls, Direction dir);
	virtual void		addWall(Wall* wall, Direction dir);

	virtual void		addAdjasentRoom(Room * room);

	virtual void		move(int x, int y);

	virtual bool		Inside(int x, int y); 
	virtual bool		Inside(Tile * t); 

	virtual void Update(Camera * cam);
	virtual void Draw() = 0;
	virtual std::string	toString() const = 0;

};

