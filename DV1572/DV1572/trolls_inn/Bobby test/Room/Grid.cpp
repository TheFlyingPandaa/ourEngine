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

Grid::Grid(int posX, int posY, int sizeX, int sizeY, Mesh * mesh)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_tileMesh = mesh;

	this->m_tiles = std::vector<std::vector<Tile*>>(this->m_sizeX);
	for (int i = 0; i < sizeX; i++)
	{
		this->m_tiles[i] = std::vector<Tile*>(this->m_sizeY);
		for (int j = 0; j < sizeY; j++)
		{
			Tile* t = new Tile(sizeX, sizeY, m_tileMesh);
			t->getQuad().setScale(2.0f);
			t->getQuad().setPos(static_cast<float>(i + posX), 0.0f, static_cast<float>(j + posY));
			t->setPosX(i);
			t->setPosY(j);
			this->m_tiles[i][j] = t;
		}
	}
	int leftCounter = 0;

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			int upright = 0;
			int upleft = 0;
			int downright = 0;
			int downleft = 0;

			for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
			{
				
				Direction dir = static_cast<Direction>(dirIndex);

				if (dir == Direction::up) {
					if (j + 1 < sizeY) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j + 1], dir);
						upright++;
						upleft++;
					}					
				}
				else if (dir == Direction::down) {
					if (j - 1 >= 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j - 1], dir);
						downright++;
						downleft++;
					}
				}
				else if (dir == Direction::left) {
					if (i - 1 >= 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j], dir);
						upleft++;
						downleft++;
					}
				}
				else if (dir == Direction::right) {
					if (i + 1 < sizeX) {
						m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j], dir);
						upright++;
						downright++;
					}
				}

				if (upright == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j + 1], static_cast<Direction>(4));
					upright = 0;
				}
				if (upleft == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j + 1], static_cast<Direction>(5));
					upleft = 0;
				}
				if (downright == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j - 1], static_cast<Direction>(6));
					downright = 0;
				}
				if (downleft == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j - 1], static_cast<Direction>(7));
					downleft = 0;
				}
			}
		}
	}

}
Grid::~Grid()
{	
	for (int i = 0; i < this->m_tiles.size(); i++)
	{
		for (int j = 0; j < this->m_tiles[i].size(); j++)
		{
			delete this->m_tiles[i][j];
		}
	}
}


std::vector<std::vector<Tile*>> Grid::getGrid() const
{
	return m_tiles;	
}



void Grid::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, bool force)
{
	if (pos.x + size.x <= m_sizeX && pos.y + size.y <= m_sizeY && pos.x >= 0 && pos.y >= 0) {

		std::vector<std::vector<Tile*>> tiles(size.x);
		for (int x = pos.x; x < size.x + pos.x; x++)
		{
			tiles[x - pos.x] = std::vector<Tile*>(size.y);
			for (int y = pos.y; y < size.y + pos.y; y++)
			{
				tiles[x - pos.x][y - pos.y] = this->m_tiles[x][y];
			}
		}

		m_roomCtrl.AddRoom(pos, size, roomType, tiles, force);
	}
}
void Grid::Draw()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		for (int j = 0; j < m_sizeY; j++)
		{
			m_tiles[i][j]->getQuad().noInside();
			m_tiles[i][j]->getQuad().Draw();
		}
	}

	m_roomCtrl.Draw();
}
void Grid::PickTiles()
{
	for (int i = 0; i < m_tiles.size(); i++)
	{
		for (int j = 0; j < m_tiles[i].size(); j++)
		{			
			m_tiles[i][j]->getQuad().setColor(1.0f,1.0f,1.0f);
			m_tiles[i][j]->getQuad().CheckPick();
		}
	}
}
bool Grid::CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(0.5f, 5.0f, 0.5f);
		
	if (end.x < start.x)
	{
		std::swap(end.x, start.x);
	}
	if (end.y < start.y)
	{
		std::swap(end.y, start.y);
	}
	
	DirectX::XMINT2 roomOffset = end;


	roomOffset.x -= start.x - 1;
	roomOffset.y -= start.y - 1;
	bool placeable = m_roomCtrl.isPlaceable(start, roomOffset);
	if (!placeable)
		color = XMFLOAT3(5.0f, 0.5f, 0.5f);

	

	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			m_tiles[i][j]->getQuad().setColor(color.x, color.y, color.z);
		}
	}
	return placeable;
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
			m_tiles[i][j]->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::DrawString()
{
	
}

RoomCtrl & Grid::getRoomCtrl()
{
	return m_roomCtrl;
}

void Grid::Update(Camera * cam) {
	m_roomCtrl.Update(cam);
}


void Grid::CreateWalls(Mesh * mesh)
{
	if (mesh)
		m_roomCtrl.setMesh(mesh);
	//m_roomCtrl.CreateWalls();
}

float Grid::getDistance(Tile* t1, Tile* t2)
{
	XMVECTOR xmTile = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmGoal = XMLoadFloat3(&t2->getQuad().getPosition());
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}

std::vector<std::shared_ptr<Node>> Grid::findPath(Tile* startTile, Tile* endTile, DirectX::XMINT2 mainDoor)
{
	std::vector<std::shared_ptr<Node>> openList;
	std::vector<std::shared_ptr<Node>> closedList;

	//std::vector<std::shared_ptr<Node>> pointerBank;

	//Node* current = new Node(startTile, nullptr, 0, getDistance(startTile, endTile));
	std::shared_ptr<Node> current(new Node(startTile, nullptr, 0, getDistance(startTile, endTile)));
	//current->tile->

	if (current->tile->getIsInside() == false && endTile->getIsInside() == true)
	{
		return findPath(startTile, m_tiles[mainDoor.x][mainDoor.y], mainDoor);
	}
	else if(current->tile->getIsInside() == true && endTile->getIsInside() == false)
	{
		return findPath(startTile, m_tiles[mainDoor.x][mainDoor.y + 1], mainDoor);
	}
	
	openList.push_back(current);

	//pointerBank.push_back(current); 

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

			/*for (auto& p : pointerBank)
				delete p;*/
			std::reverse(path.begin(), path.end());
			return path;
		}
		
		closedList.push_back(current);		// add the entry to the closed list
		openList.erase(openList.begin());   // Remove the entry

		for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
		{
			
			Direction dir = static_cast<Direction>(dirIndex);

			Tile* currentTile = current->tile->getAdjacent(dir);

			// Rules here
			if (currentTile == nullptr)
				continue;
			if (current->tile->getIsInside())
			{
				if (endTile->getIsInside() == false)
				{
					continue;
				}
			}
			else {
				if (currentTile->getRoom() != nullptr)
					continue; // Jump this one
			}
			if (!currentTile->getIsWalkeble())
				continue;
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

Tile* Grid::getTile(int x, int y) const
{
	if (x < 0 || x > m_sizeX || y < 0 || y >m_sizeY)
		return nullptr;
	return m_tiles[x][y];
}

