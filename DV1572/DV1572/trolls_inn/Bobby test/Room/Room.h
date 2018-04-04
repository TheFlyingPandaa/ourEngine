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
	//Tile**	m_tiles;
	std::vector<std::vector<Tile*>> m_tiles;

	std::vector<Wall*> up;
	std::vector<Wall*> down;
	std::vector<Wall*> left;
	std::vector<Wall*> right;

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr);	
	Room(int posX, int posY , int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles);

	virtual ~Room();	

	virtual std::vector<std::vector<Tile*>>		getTiles() const; 
	virtual Tile*								getTiles(int x, int y) const;


	virtual int			getX() const; 
	virtual int			getY() const;
	virtual int			getSizeX() const; 
	virtual int			getSizeY() const; 

	virtual void		move(int x, int y);
	//virtual bool		getHasWalls() const;
	//virtual void		CreateWalls(Mesh * mesh);

	virtual bool		Inside(int x, int y); 
	virtual bool		Inside(Tile * t); 

	virtual void Draw() = 0;
	virtual std::string	toString() const = 0;

};

