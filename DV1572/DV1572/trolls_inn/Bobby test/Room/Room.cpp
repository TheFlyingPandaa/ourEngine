#include "Room.h"

Room::Room(int posX, int posY, int sizeX, int sizeY, Mesh * m)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;
	
	//this->m_hasDoor = false;

	for (unsigned short x = 0; x < m_tiles.size(); x++)
	{
		for (unsigned short y = 0; y < m_tiles[x].size(); y++)
		{
			//8D
			//To scale
			m_tiles[x][y]->setMesh(m);
		}
	}
	//TODO: Dont forget to send the tiles you lil cunt :D
}

Room::Room(int posX, int posY, int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_tiles = tiles;

	//this->m_hasDoor = false;

	for (unsigned short x = 0; x < tiles.size(); x++)
	{
		for (unsigned short y = 0; y < tiles[x].size(); y++)
		{
			tiles[x][y]->setInside(true);
			tiles[x][y]->setIsWalkeble(true);
			tiles[x][y]->setRoom(this);

		}
	}
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

void Room::setTile(Mesh * mesh)
{
	for (int x = 0; x < m_tiles.size(); x++)
	{
		for (int y = 0; y < m_tiles[x].size(); y++)
		{
			m_tiles[x][y]->setMesh(mesh);
			//m_tiles[x][y]->quad.setScale(1, 1, 1);
			//m_tiles[x][y]->quad.setScale(1, 1, 1);
		}
	}
}



bool Room::Inside(int x, int y)
{
	return	x >= m_posX && x < m_posX + m_sizeX &&
			y >= m_posY && y < m_posY + m_sizeY;
}

bool Room::Inside(Tile * t)
{
	return	t->getPosX() >= m_posX && t->getPosY() < m_posX + m_sizeX &&
			t->getPosY() >= m_posY && t->getPosY() < m_posY + m_sizeY;
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
			cullWalls[Direction::left] = true;
		}
		else if (camPosition.x > m_posX + m_sizeX)
		{
			cullWalls[Direction::right] = true;
		}
		if (camPosition.y < m_posY)
		{
			cullWalls[Direction::down] = true;
		}
		else if (camPosition.y > m_posY + m_sizeY)
		{
			cullWalls[Direction::up] = true;
		}

		bool changeUp = false;
		bool changeDown = false;
		bool changeRight = false;
		bool changeLeft = false;

		if (m_culledWalls[Direction::up] != cullWalls[Direction::up])
		{
			m_culledWalls[Direction::up] = cullWalls[Direction::up];
			changeUp = true;
		}
		if (m_culledWalls[Direction::down] != cullWalls[Direction::down])
		{
			m_culledWalls[Direction::down] = cullWalls[Direction::down];
			changeDown = true;
		}
		if (m_culledWalls[Direction::right] != cullWalls[Direction::right])
		{
			m_culledWalls[Direction::right] = cullWalls[Direction::right];
			changeRight = true;
		}
		if (m_culledWalls[Direction::left] != cullWalls[Direction::left])
		{
			m_culledWalls[Direction::left] = cullWalls[Direction::left];
			changeLeft = true;
		}


		if (changeUp || changeDown)
		{
			for (size_t x = 0; x < down.size(); x++)
			{
				if (changeDown)
				{
					if (m_culledWalls[Direction::down] && !down[x]->getIsDoor())
						down[x]->setScale(1.0f, 0.05f, 1.0f);
					else
						down[x]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t x = 0; x < up.size(); x++)
			{
				if (changeUp)
				{
					if (m_culledWalls[Direction::up] && !up[x]->getIsDoor())
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
					if (m_culledWalls[Direction::left] && !left[y]->getIsDoor())
						left[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						left[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t y = 0; y < right.size(); y++)
			{
				if (changeRight)
				{
					if (m_culledWalls[Direction::right] && !right[y]->getIsDoor())
						right[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						right[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
		}

	}
	for (int i = 0; i < down.size(); i++)
	{
		if (down[i]->getIsInner() && !down[i]->getIsDoor())
			down[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < up.size(); i++)
	{
		if (up[i]->getIsInner() && !up[i]->getIsDoor())
			up[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < left.size(); i++)
	{
		if (left[i]->getIsInner() && !left[i]->getIsDoor())
			left[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < right.size(); i++)
	{
		if (right[i]->getIsInner() && !right[i]->getIsDoor())
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

void Room::setWalls(std::vector<Wall*> walls, Direction dir)
{
	for (int i = 0; i < walls.size(); i++)
	{
		m_allWalls.push_back(walls[i]);
	}

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

void Room::addWall(Wall * wall, Direction dir)
{
	m_allWalls.push_back(wall);
	switch (dir)
	{
	case 0:		
		if (std::find(up.begin(), up.end(), wall) == up.end())
			up.push_back(wall);
		break;
	case 1:
		if (std::find(down.begin(), down.end(), wall) == down.end())
			down.push_back(wall);
		break;
	case 2:
		if (std::find(left.begin(), left.end(), wall) == left.end())
			left.push_back(wall);
		break;
	case 3:
		if (std::find(right.begin(), right.end(), wall) == right.end())
			right.push_back(wall);
		break;
	default:
		break;
	}
}

DirectX::XMFLOAT3 Room::getPosition() const
{
	return DirectX::XMFLOAT3(static_cast<float>(getX()), 0.0f, static_cast<float>(getY()));
}

void Room::addAdjasentRoom(Room * room)
{
	if (std::find(adjasent.begin(), adjasent.end(), room) == adjasent.end())
		adjasent.push_back(room);
}

std::vector<Room*>* Room::getAdjasent()
{
	return &adjasent;
}

std::vector<Wall*>* Room::getAllWalls()
{
	return &m_allWalls;
}

std::vector<Wall*>* Room::getWall(Direction dir)
{
	switch (dir)
	{
	case Direction::up:
		return &up;
		break;
	case Direction::down:
		return &down;
		break;
	case Direction::left:
		return &left;
		break;
	case Direction::right:
		return &right;
		break;
	default:
		return nullptr;
		break;
	}
}

void Room::setHasDoor(Direction dir, bool value)
{
	this->m_hasDoor[dir] = value;
}

bool Room::getHasDoor(Direction dir) const
{
	return this->m_hasDoor[dir];
}

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;
}
