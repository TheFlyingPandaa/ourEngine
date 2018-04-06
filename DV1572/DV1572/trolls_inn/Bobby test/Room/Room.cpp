#include "Room.h"

Room::Room(int posX, int posY, int sizeX, int sizeY, Mesh * m)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	//TODO: Dont forget to send the tiles you lil cunt :D
}

Room::Room(int posX, int posY, int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_tiles = tiles;
}

Room::~Room()
{

}

std::vector<std::vector<Tile*>> Room::getTiles() const
{
	return this->m_tiles;
}

Tile * Room::getTiles(int x, int y) const
{
	if (x >= 0 && x < this->m_sizeX && y >= 0 && y < this->m_sizeY)
		return m_tiles[x][y];
	else
		return nullptr;
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

void Room::Update(Camera * cam)
{
	
	if (cam)
	{
		bool cullWalls[4] = { false, false, false, false };
		DirectX::XMFLOAT3 camPos3D = cam->getPosition();
		DirectX::XMFLOAT2 camPosition = { camPos3D.x, camPos3D.z };

		if (camPosition.x < m_posX)
		{
			cullWalls[WallDirection::left] = true;
		}
		else if (camPosition.x > m_posX + m_sizeX)
		{
			cullWalls[WallDirection::right] = true;
		}
		if (camPosition.y < m_posY)
		{
			cullWalls[WallDirection::down] = true;
		}
		else if (camPosition.y > m_posY + m_sizeY)
		{
			cullWalls[WallDirection::up] = true;
		}

		bool changeUp = false;
		bool changeDown = false;
		bool changeRight = false;
		bool changeLeft = false;

		if (m_culledWalls[WallDirection::up] != cullWalls[WallDirection::up])
		{
			m_culledWalls[WallDirection::up] = cullWalls[WallDirection::up];
			changeUp = true;
		}
		if (m_culledWalls[WallDirection::down] != cullWalls[WallDirection::down])
		{
			m_culledWalls[WallDirection::down] = cullWalls[WallDirection::down];
			changeDown = true;
		}
		if (m_culledWalls[WallDirection::right] != cullWalls[WallDirection::right])
		{
			m_culledWalls[WallDirection::right] = cullWalls[WallDirection::right];
			changeRight = true;
		}
		if (m_culledWalls[WallDirection::left] != cullWalls[WallDirection::left])
		{
			m_culledWalls[WallDirection::left] = cullWalls[WallDirection::left];
			changeLeft = true;
		}


		if (changeUp || changeDown)
		{
			for (size_t x = 0; x < down.size(); x++)
			{
				if (changeDown)
				{
					if (m_culledWalls[WallDirection::down])
						down[x]->setScale(1.0f, 0.05f, 1.0f);
					else
						down[x]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t x = 0; x < up.size(); x++)
			{
				if (changeUp)
				{
					if (m_culledWalls[WallDirection::up])
						up[x]->setScale(1.0f, 0.05f, 1.0f);
					else
						up[x]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
		}

		if (changeRight || changeLeft)
		{
			for (size_t y = 0; y < left.size(); y++)
			{
				if (changeLeft)
				{
					if (m_culledWalls[WallDirection::left])
						left[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						left[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t y = 0; y < right.size(); y++)
			{
				if (changeRight)
				{
					if (m_culledWalls[WallDirection::right])
						right[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						right[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
		}

	}
	for (int i = 0; i < down.size(); i++)
	{
		if (down[i]->getIsInner())
			down[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < up.size(); i++)
	{
		if (up[i]->getIsInner())
			up[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < left.size(); i++)
	{
		if (left[i]->getIsInner())
			left[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < right.size(); i++)
	{
		if (right[i]->getIsInner())
			right[i]->setScale(1.0f, 0.05f, 1.0f);
	}
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

void Room::setWalls(std::vector<Wall*> walls, WallDirection dir)
{
	switch (dir)
	{
	case 0:
		up = walls;
		break;
	case 1:
		down = walls;
		break;
	case 2:
		left = walls;
		break;
	case 3:
		right = walls;
		break;
	default:
		break;
	}
}

void Room::addWall(Wall * wall, WallDirection dir)
{
	switch (dir)
	{
	case 0:
		up.push_back(wall);
		break;
	case 1:
		down.push_back(wall);
		break;
	case 2:
		left.push_back(wall);
		break;
	case 3:
		right.push_back(wall);
		break;
	default:
		break;
	}
}

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;

	for (size_t i = 0; i < up.size(); i++)
	{
		
	}
}

/*bool Room::getHasWalls() const
{
	return this->m_hasWalls;
}*/
/*
void Room::CreateWalls(Mesh * mesh)
{
	this->m_wall = mesh;
	this->m_hasWalls = true;

	for (size_t x = 0; x < m_sizeX; x++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY);
		//obj->setScale(0.5f);
		down.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY + m_sizeY);
		//obj->setScale(0.5f);
		up.push_back(obj);
	}

	for (size_t y = 0; y < m_sizeY; y++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		//obj->setScale(0.5f);
		left.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);


		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		//obj->setScale(0.5f);
		right.push_back(obj);
	}
}
*/