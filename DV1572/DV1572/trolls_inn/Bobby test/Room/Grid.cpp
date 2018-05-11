#include "Grid.h"
#include <math.h>
#include <memory>
#include <algorithm>
#include <chrono>
#include <unordered_map>

bool Grid::_findInVec(std::vector<Node*>& list, Node * node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}
std::vector<std::shared_ptr<Node>>::iterator Grid::_findInVec(std::vector<std::shared_ptr<Node>>& list, const std::shared_ptr<Node>& node) const
{
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		if (*(*it) == *node)
			return it;
	}
	return list.end();
	/*	if (*l == *node)
			return l;
	auto first = std::lower_bound(list.begin(), list.end(), node, [](const std::shared_ptr<Node>& a1, const std::shared_ptr<Node> &a2) {return a1->fCost < a2->fCost; });
	if ((!(first == list.end()) && !(node->fCost < (*first)->fCost)))
		return first;
	else
		return list.end();*/
}

int binSearch(const std::vector<std::shared_ptr<Node>>& list, int left, int right, std::shared_ptr<Node> node)
{
	if (right >= left)
	{
		int mid = left + (right - left) / 2;

		// If the element is present at the middle 
		// itself
		if (*list[mid] == *node)
			return mid;

		// If element is smaller than mid, then 
		// it can only be present in left subarray
		if (*node < *list[mid])
			return binSearch(list, left, mid - 1, node);

		// Else the element can only be present
		// in right subarray
		return binSearch(list, mid + 1, right, node);
	}

	// We reach here when element is not 
	// present in array
	return -1;
}

int Grid::_index(int x, int y) const
{
	if (x < 0 || x >= m_sizeX) return -1;
	if (y < 0 || y >= m_sizeY) return -1;

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
			t->getQuad().setUVScaleX(1);
			t->getQuad().setUVScaleY(1);

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

bool Grid::MarkAllTilesRed(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(8.0f, 0.5f, 0.5f);

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
				color = XMFLOAT3(8.0f, 0.5f, 0.5f);
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
	return false;
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

	//XMFLOAT3 targetPosition;
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
	XMFLOAT3 node = t1->getQuad().getPosition();
	XMFLOAT3 goal = t2->getQuad().getPosition();
	float dx = std::abs(node.x - goal.x);
	float dy = std::abs(node.z - goal.z);
	return 10 * (dx + dy) + (14.14f - 2 * 10.0f) * min(dx, dy);
}

std::vector<std::shared_ptr<Node>> Grid::findPath(Tile* startTile, Tile* endTile)
{
	auto getAdjacentTile = [&](std::shared_ptr<Node> current, float dx, float dy) -> Tile*
	{
		int index = _index(static_cast<int>(current->tile->getQuad().getPosition().x + dx), static_cast<int>(current->tile->getQuad().getPosition().z + dy));
		if (index < 0 || index >= m_tiles.size()) return nullptr;;
		return m_tiles[index];
	};

	std::vector<std::shared_ptr<Node>> openList;

	bool** closedList3 = new bool*[m_sizeX];
	for (int i = 0; i < m_sizeX; i++)
	{
		closedList3[i] = new bool[m_sizeY];
		for (int j = 0; j < m_sizeY; j++)
			closedList3[i][j] = 0;	

	}

	std::shared_ptr<Node> current(new Node(startTile, nullptr, 0, getDistance(startTile, endTile)));
	
	openList.push_back(current);
	auto start = std::chrono::system_clock::now();
	while (openList.size())
	{
		std::sort(openList.begin(), openList.end(), [](const std::shared_ptr<Node>& a1, const std::shared_ptr<Node>& a2) {return a1->fCost < a2->fCost; });
		current = openList.front();

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
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaTime = end - start;
			std::cout << deltaTime.count() << std::endl;
			return path;
		}
		closedList3[current->tile->getPosX()][current->tile->getPosY()] = 1;
		//closedList.push_back(current);		// add the entry to the closed list
		openList.erase(openList.begin());   // Remove the entry

		for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
		{
			
			Direction dir = static_cast<Direction>(dirIndex);
			float addedCost = (dirIndex > 3) ? 14.14f : 10.0f;
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
			}
			int index = _index(static_cast<int>(current->tile->getQuad().getPosition().x + dirFloat.x), static_cast<int>(current->tile->getQuad().getPosition().z + dirFloat.y));
			if (index < 0 || index >= m_tiles.size()) continue;
			Tile* currentTile = m_tiles[index];
			
			// Rules here

			if (currentTile == nullptr)
				continue;
			

			if (dir == Direction::downleft)
			{
				Tile* leftTile = getAdjacentTile(current, -1, 0);
				if (leftTile == nullptr) continue;

				Tile* downTile = getAdjacentTile(current, 0, -1);
				if (downTile == nullptr) continue;
			}
			else if (dir == Direction::downright)
			{
				Tile* rightTile = getAdjacentTile(current, 1, 0);
				if (rightTile == nullptr) continue;

				Tile* downTile = getAdjacentTile(current, 0, -1);
				if (downTile == nullptr) continue;
			}
			else if (dir == Direction::upright)
			{

				Tile* upTile = getAdjacentTile(current, 0, 1);
				if (upTile == nullptr) continue;

				Tile* rightTile = getAdjacentTile(current, 1, 0);
				if (rightTile == nullptr) continue;
			}
			else if (dir == Direction::upleft)
			{

				Tile* upTile = getAdjacentTile(current, 0, 1);
				if (upTile == nullptr) continue;

				Tile* leftTile = getAdjacentTile(current, -1, 0);
				if (leftTile == nullptr) continue;
			}
		
			
			currentTile->getQuad().setColor(255, 0, 0);

			//--Rules End Here--

			float gCost = current->gCost + addedCost;

			float hCost = getDistance(currentTile, endTile);
			std::shared_ptr<Node> newNode (new Node(currentTile, current, gCost, hCost));
			//pointerBank.push_back(newNode);
			if (closedList3[newNode->tile->getPosX()][newNode->tile->getPosY()] && gCost >= newNode->gCost)
			{
				continue;
			}

			auto vecIndex = _findInVec(openList ,newNode);
			//auto vecIndex = binSearch(openList, 0, openList.size() - 1, newNode);

			if (vecIndex == openList.end() || gCost < newNode->gCost)
		//	if (vecIndex == -1 || gCost < newNode->gCost)
			{
				openList.push_back(newNode);
				
			}
			else if (gCost < (*vecIndex)->gCost)
			{
				(*vecIndex)->parent = current;
				(*vecIndex)->gCost = gCost;
			}
			/*else if (gCost < openList[vecIndex]->gCost)
			{
				openList[vecIndex]->parent = current;
				openList[vecIndex]->gCost = gCost;
				std::cout << "LOL" << std::endl;
			}*/

			
			
		}
		
		
	}
	
	return std::vector<std::shared_ptr<Node>>();
}
