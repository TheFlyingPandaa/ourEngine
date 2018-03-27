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
		//m_sizeX = sizeX;
		//m_sizeY = sizeY;
		m_door = false;
		m_room = room;
		quad.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
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

	Mesh*	m_wall;

	std::vector<Object3D*> up;
	std::vector<Object3D*> down;
	std::vector<Object3D*> left;
	std::vector<Object3D*> right;

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, Mesh * m = nullptr){
		this->m_posX = posX;
		this->m_posY = posY;
		this->m_sizeX = sizeX;
		this->m_sizeY = sizeY;

		this->m_wall = m;

		for (size_t x = 0; x < sizeX; x++)
		{
			Object3D * obj = new Object3D();
			obj->setMesh(m_wall);

			obj->setPos(x + 0.5f, 0, 0);
			obj->setScale(0.5f);
			down.push_back(obj);

			obj = new Object3D();
			obj->setMesh(m_wall);

			obj->setPos(x + 0.5f, 0, sizeY);
			obj->setScale(0.5f);
			up.push_back(obj);
		}

		for (size_t y = 0; y < sizeY; y++)
		{
			Object3D * obj = new Object3D();
			obj->setMesh(m_wall);

			obj->setRotation(0, 90, 0);
			obj->setPos(0, 0, y + 0.5f);
			obj->setScale(0.5f);
			left.push_back(obj);

			obj = new Object3D();
			obj->setMesh(m_wall);


			obj->setRotation(0, 90, 0);
			obj->setPos(sizeX, 0, y + 0.5f);
			obj->setScale(0.5f);
			right.push_back(obj);
		}


		//TODO: Dont forget to send the tiles you lil cunt :D
	}
	virtual ~Room() {
		for (size_t i = 0; i < up.size(); i++)
		{
			delete up[i];
			delete down[i];
		}
		for (size_t i = 0; i < left.size(); i++)
		{
			delete left[i];
			delete right[i];
		}
	};

	virtual Tile**		getTiles() const { return m_tiles; }
	virtual int			getX() const { return m_posX; }
	virtual int			getY() const { return m_posY; }
	virtual int			getSizeX() const { return m_sizeX; }
	virtual int			getSizeY() const { return m_sizeY; }

	virtual bool		Inside(int x, int y) {
		return	x >= m_posX && x < m_posX + m_sizeX && 
				y >= m_posY && y < m_posY + m_sizeY;
	}
	virtual bool		Inside(Tile * t) {
		return	t->m_posX >= m_posX && t->m_posY < m_posX + m_sizeX &&
				t->m_posY >= m_posY && t->m_posY < m_posY + m_sizeY;
	}

	virtual void Draw() = 0;
	virtual std::string	toString() const = 0;

};

