#include "Grid.h"
#include <math.h>
#include <memory>

bool Grid::_findInVec(std::vector<Node*>& list, Node * node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}
bool Grid::_findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}

int Grid::_index(int x, int y) const
{
	return x + (y * m_sizeY);
}


Grid::Grid(int posX, int posY, int sizeX, int sizeY, Mesh * mesh)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;
	
	m_catWalkMesh = new Mesh();
	this->m_gridMesh = new Mesh();
	m_tileMesh = new Mesh();

	m_gridMesh->MakeRectangle();
	m_gridMesh->setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_gridMesh->setNormalTexture("trolls_inn/Resources/GrassNormal.png");

	m_tileMesh->MakeRectangle();
	m_tileMesh->setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_tileMesh->setNormalTexture("trolls_inn/Resources/GrassNormal.png");

	m_catWalkMesh->MakeRectangle();
	
	m_catWalkMesh->setDiffuseTexture("trolls_inn/Resources/Brickfloor/brickwall.png");
	m_catWalkMesh->setNormalTexture("trolls_inn/Resources/Brickfloor/brickwall_normal.png");

	m_wholeGrid.setMesh(m_gridMesh);
	m_wholeGrid.setScale(sizeX *2.0f);
	m_wholeGrid.setPos(static_cast<float>(posX), -0.01f, static_cast<float>(posY));
	m_wholeGrid.setRotation(90.0f, 0.0f, 0.0f);
	m_wholeGrid.setUVScale(sizeX);

	m_catWalkTile.setMesh(m_catWalkMesh);
	m_catWalkTile.setScale(sizeX*2.0f, 1, 10.0f);
	m_catWalkTile.setPos(static_cast<float>(posX), -0.01f, static_cast<float>(posY - 5));
	m_catWalkTile.setRotation(90.0f, 0.0f, 0.0f);
	m_catWalkTile.setUVScaleX(sizeX);
	m_catWalkTile.setUVScaleY(10);

	this->m_tiles.reserve(sizeX * sizeY);

	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			Tile* t = new Tile(sizeX, sizeY, m_tileMesh);

			t->getQuad().setScale(2.0f);
			t->getQuad().setPos(static_cast<float>(j + posX), 0, static_cast<float>(i + posY));
			t->getQuad().setUVScaleX(1.0f);
			t->getQuad().setUVScaleY(1.0f);

			this->m_tiles.push_back(t);
		}
	}
}
Grid::~Grid()
{
	for (auto& tile : m_tiles)
		delete tile;

	delete m_gridMesh;
	delete m_tileMesh;
	delete m_catWalkMesh;
}


std::vector<Tile*> Grid::getGrid() const
{
	return m_tiles;	
}

//bool Grid::CheckIfDoorCanBeBuilt(DirectX::XMINT2 pos, DirectX::XMINT2 pos2)
//{
//	// TODO
//
//	/*bool result = false;
//	DirectX::XMFLOAT3 c(5.0f, 0.5f, 0.5f);
//	Tile* t1 = m_tiles[pos.x][pos.y];
//	Tile* t2 = m_tiles[pos2.x][pos2.y];
//	if ((abs(pos.x - pos2.x) == 1 || abs(pos.y - pos2.y) == 1) &&
//		(pos.x == pos2.x || pos.y == pos2.y))
//	{
//		if (t1->getIsInside() && t2->getIsInside())
//		{
//			if (t1 != t2)
//			{
//				Direction dir = m_roomCtrl.getDirection(t1, t2);
//
//				Wall* w = t1->getTileWalls(dir);
//				if (w)
//				{
//					c = DirectX::XMFLOAT3(0.05f, 5.0f, 0.5f);
//				}
//			}
//		}
//	}
//	
//	t1->getQuad().setColor(c.x, c.y, c.z);
//	t2->getQuad().setColor(c.x, c.y, c.z);
//
//	return result;*/
//}


//void Grid::AddRoomObject(DirectX::XMINT2 pos, Mesh * mesh)
//{
//	// TODO
//	/*m_tiles[pos.x][pos.y]->setIsWalkeble(false);
//	m_tiles[pos.x][pos.y]->setHasObject(true);
//
//	m_roomCtrl.addRoomObject(pos, mesh);*/
//}

std::vector<Tile*> Grid::extractTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size)
{
	std::vector<Tile*> extractedTiles;
	extractedTiles.reserve(size.x * size.y);

	for (int _y = 0; _y < size.y; _y++)
	{
		for (int _x = 0; _x < size.x; _x++)
		{
			Tile* tile = m_tiles[_index(_x + pos.x, _y + pos.y)];

			if (tile != nullptr)
			{
				extractedTiles.push_back(tile);
				m_tiles[_index(_x + pos.x, _y + pos.y)] = nullptr;
			}

		}
	}

	return extractedTiles;
}

void Grid::insertTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size, std::vector<Tile*>& tiles)
{
	int counter = 0;
	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			m_tiles[_index(x + pos.x, y + pos.y)] = tiles[counter++];
			m_tiles[_index(x + pos.x, y + pos.y)]->setMesh(m_tileMesh);
			m_tiles[_index(x + pos.x, y + pos.y)]->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

Tile * Grid::getTile(int x, int y)
{
	return m_tiles[_index(x,y)];
}

void Grid::Draw()
{
	for (auto& tile : m_tiles)
	{
		if (tile != nullptr)
		{
			if (tile->getQuad().getColor().x != 1.0f)
				tile->getQuad().Draw();

		}
	}
	m_catWalkTile.Draw();
	m_wholeGrid.Draw();

}

void Grid::PickTiles(Shape* selectionTile)
{
	bool notFound = false;
	if (selectionTile)
	{
		int xPos = static_cast<int>(selectionTile->getPosition().x);
		int yPos = static_cast<int>(selectionTile->getPosition().z);
		for (int x = -5; x < 5; x++)
		{

			for (int y = -5; y < 5; y++)
			{
				int indexX = xPos + x;
				int indexY = yPos + y;

				// Boundcheck
				if (indexX < 0 || indexX >= m_sizeX || indexY < 0 || indexY >= m_sizeY) continue;
				Tile* t = m_tiles[_index(indexX, indexY)];
				if(t)t->getQuad().CheckPick();
			}
			
		}

		for (auto& t : m_tiles)
			if (t)
				t->getQuad().setColor(1.0f, 1.0f, 1.0f);

	}
	else
	{
		for (int i = 0; i < m_tiles.size(); i++)
		{
			if (m_tiles[i] != nullptr)
			{
				m_tiles[i]->getQuad().setColor(1.0f, 1.0f, 1.0f);
				m_tiles[i]->getQuad().CheckPick();
			}
		}
	}
	
	
}
bool Grid::CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(0.5f, 500.0f, 0.5f);

	if (end.x < start.x)
	{
		std::swap(end.x, start.x);
	}
	if (end.y < start.y)
	{
		std::swap(end.y, start.y);
	}
	

	

	bool result = true;
	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if (!t)
			{
				color = XMFLOAT3(5.0f, 0.5f, 0.5f);
				result = false;
			}
		}
	}
	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if (t)
				t->getQuad().setColor(color.x, color.y, color.z);
		}
	}

	return result;
}



void Grid::ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end)
{
	if (end.x < pos.x)
	{
		std::swap(end.x, pos.x);
	}
	if (end.y < pos.y)
	{
		std::swap(end.y, pos.y);
	}
	for (int i = pos.x; i < end.x + 1; i++)
	{
		for (int j = pos.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if(t)
				t->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::DrawString()
{
	
}
void Grid::generatePath(Character& character, RoomType targetRoom)
{
	auto round_n = [&](float num, int dec) -> float
	{
		float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
		float pwr = pow(10.0f, dec);
		return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
	};
	XMFLOAT2 charPos = character.getPosition(); // (x,y) == (x,z,0)

	XMFLOAT3 targetPosition;
	/*if (targetRoom == randomStupid)
		targetPosition = { float(rand() % 31), 0.0f, float(rand() % 31) };
	else
		targetPosition = m_roomCtrl.getClosestRoom(charPos, targetRoom);*/


	int xTile = (int)(round_n(charPos.x, 1) - 0.5f);
	int yTile = (int)(round_n(charPos.y, 1) - 0.5f);

//	std::vector<std::shared_ptr<Node>> path = findPathHighLevel(m_tiles[xTile][yTile], m_tiles[targetPosition.x][targetPosition.z]);

	XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

	//if (path.size() != 0)
	//{

	//	character.Move(character.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

	//	for (int i = 0; i < path.size() - 1; i++)
	//		character.Move(character.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
	//}
}

float Grid::getDistance(Tile* t1, Tile* t2)
{
	XMVECTOR xmTile = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmGoal = XMLoadFloat3(&t2->getQuad().getPosition());
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}

std::vector<std::shared_ptr<Node>> Grid::findPath(Tile* startTile, Tile* endTile)
{
	std::vector<std::shared_ptr<Node>> openList;
	std::vector<std::shared_ptr<Node>> closedList;

	std::shared_ptr<Node> current(new Node(startTile, nullptr, 0, getDistance(startTile, endTile)));
	
	openList.push_back(current);

	while (openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), [](std::shared_ptr<Node> a1, std::shared_ptr<Node> a2) {return a1->fCost < a2->fCost; });
		current = openList.at(0);

		if (*current == *endTile)
		{
			std::vector<std::shared_ptr<Node>> path;
			while (current->parent != nullptr)
			{
				path.push_back(current);
				std::shared_ptr<Node> t(current->parent);
				current = t;
				
			}

			std::reverse(path.begin(), path.end());
			return path;
		}
		
		closedList.push_back(current);		// add the entry to the closed list
		openList.erase(openList.begin());   // Remove the entry

		for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
		{
			
			Direction dir = static_cast<Direction>(dirIndex);

			XMFLOAT2 dirFloat;
			switch (dir)
			{
			case up:
				dirFloat = XMFLOAT2(0, 1);
				break;
			case down:
				dirFloat = XMFLOAT2(0, -1);
				break;
			case left:
				dirFloat = XMFLOAT2(-1, 0);
				break;
			case right:
				dirFloat = XMFLOAT2(1, 0);
				break;
			case upright:
				dirFloat = XMFLOAT2(1, 1);
				break;
			case upleft:
				dirFloat = XMFLOAT2(-1, 1);
				break;
			case downright:
				dirFloat = XMFLOAT2(1, -1);
				break;
			case downleft:
				dirFloat = XMFLOAT2(-1, -1);
				break;
			case noneSpecial:
				break;
			default:
				break;
			}
			int index = _index(current->tile->getQuad().getPosition().x + dirFloat.x, current->tile->getQuad().getPosition().z + dirFloat.y);
			if (index < 0) continue;
			Tile* currentTile = m_tiles[index];
			
			// Rules here

			if (currentTile == nullptr)
				continue;
			
			/*if (dir == Direction::downleft)
			{
				if (current->tile->getAdjacent(left)->getRoom() != nullptr)
					continue;
				if (current->tile->getAdjacent(down)->getRoom() != nullptr)
					continue;
			}
			else if (dir == Direction::downright)
			{
				if (current->tile->getAdjacent(right)->getRoom() != nullptr)
					continue;
				if (current->tile->getAdjacent(down)->getRoom() != nullptr)
					continue;
			}
			else if (dir == Direction::upright)
			{
				if (current->tile->getAdjacent(up)->getRoom() != nullptr)
					continue;
				if (current->tile->getAdjacent(right)->getRoom() != nullptr)
					continue;
			}
			else if (dir == Direction::upleft)
			{
				if (current->tile->getAdjacent(up)->getRoom() != nullptr)
					continue;
				if (current->tile->getAdjacent(left)->getRoom() != nullptr)
					continue;
			}*/
		
			


			//--Rules End Here--

			float gCost = current->gCost + (getDistance(current->tile, currentTile) == 1 ? 1 : 0.95f);

			float hCost = getDistance(currentTile, endTile);
			std::shared_ptr<Node> newNode (new Node(currentTile, current, gCost, hCost));
			//pointerBank.push_back(newNode);

			if (_findInVec(closedList,newNode) && gCost >= newNode->gCost)
				continue;
			if (!_findInVec(openList, newNode) || gCost < newNode->gCost)
			{
				openList.push_back(newNode);
				
			}
			
			
		}
		
		
	}
	
	return std::vector<std::shared_ptr<Node>>();
}
