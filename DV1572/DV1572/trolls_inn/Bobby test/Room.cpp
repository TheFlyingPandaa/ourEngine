#include "Room.h"

Room::Room(int posX, int posY, int sizeX, int sizeY, Mesh * m)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;
	this->m_hasWalls = false;
	
	this->m_wall = m;

	if (m)
		CreateWalls(m);

	//TODO: Dont forget to send the tiles you lil cunt :D
}

Room::~Room()
{
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
}

Tile ** Room::getTiles() const
{
	return m_tiles;
}

bool Room::Inside(int x, int y)
{
	return	x >= m_posX && x < m_posX + m_sizeX &&
			y >= m_posY && y < m_posY + m_sizeY;
}

bool Room::Inside(Tile * t)
{
	return	t->m_posX >= m_posX && t->m_posY < m_posX + m_sizeX &&
			t->m_posY >= m_posY && t->m_posY < m_posY + m_sizeY;
}

int Room::getX() const
{
	return m_posX;
}

int Room::getY() const
{
	return m_posY;
}

int Room::getSizeX() const
{
	return m_sizeX;
}

int Room::getSizeY() const
{
	return m_sizeY;
}

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;

	for (size_t i = 0; i < up.size(); i++)
	{
		
	}
}

bool Room::getHasWalls() const
{
	return this->m_hasWalls;
}

void Room::CreateWalls(Mesh * mesh)
{
	this->m_wall = mesh;
	this->m_hasWalls = true;

	for (size_t x = 0; x < m_sizeX; x++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY);
		obj->setScale(0.5f);
		down.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY + m_sizeY);
		obj->setScale(0.5f);
		up.push_back(obj);
	}

	for (size_t y = 0; y < m_sizeY; y++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		obj->setScale(0.5f);
		left.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);


		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		obj->setScale(0.5f);
		right.push_back(obj);
	}
}
