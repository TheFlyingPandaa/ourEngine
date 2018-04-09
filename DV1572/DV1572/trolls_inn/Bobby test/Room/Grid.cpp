#include "Grid.h"
#include <math.h>


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
			Tile* t = new Tile(posX, posY, sizeX, sizeY, m_tileMesh);
			t->quad.setScale(2.0f);
			t->quad.setPos(static_cast<float>(i + posX), 0.0f, static_cast<float>(j + posY));
			this->m_tiles[i][j] = t;
		}
	}
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (k == Direction::up) {
					if (j + 1 < sizeY) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j + 1], static_cast<Direction>(k));
					}					
				}
				else if (k == Direction::down) {
					if (j - 1 > 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j - 1], static_cast<Direction>(k));
					}
				}
				else if (k == Direction::left) {
					if (i - 1 > 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j], static_cast<Direction>(k));
					}
				}
				else if (k == Direction::right) {
					if (i + 1 < sizeX) {
						m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j], static_cast<Direction>(k));
					}
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


Tile ** Grid::getGrid() const
{
	//TODO
	return nullptr;	
}

std::vector<std::vector<Tile*>> Grid::getTiles() const
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
			m_tiles[i][j]->quad.Draw();
		}
	}

	m_roomCtrl.Draw();
}
void Grid::PickTiles()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		for (int j = 0; j < m_sizeY; j++)
		{
			
			m_tiles[i][j]->quad.setColor(1.0f,1.0f,1.0f);
			m_tiles[i][j]->quad.CheckPick();
		}
	}
}
bool Grid::CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(0.1f, 1.0f, .1f);
		
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
		color = XMFLOAT3(1.0f, 0.3f, 0.3f);

	

	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			m_tiles[i][j]->quad.setColor(color.x, color.y, color.z);
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
			m_tiles[i][j]->quad.setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::DrawString()
{
	
}

void Grid::Update(Camera * cam) {
	m_roomCtrl.Update(cam);
}


void Grid::CreateWalls(Mesh * mesh)
{
	if (mesh)
		m_roomCtrl.setMesh(mesh);
	m_roomCtrl.CreateWalls();
}

float Grid::getDistance(const Tile& t1, const Tile& t2) const
{
	XMVECTOR xmTile = XMLoadFloat2(&t1.getPosition());
	XMVECTOR xmGoal = XMLoadFloat2(&t2.getPosition());
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}

std::vector<Node*> Grid::findPath(Tile startTile, Tile endTile) const
{
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	std::vector<Node*> pointerBank;
	Node* current = new Node(&startTile, nullptr, 0, getDistance(startTile, endTile));
	pointerBank.push_back(current);
	openList.push_back(current);
	while (openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), [](const Node* n1, const Node* n2) -> bool
		{ return *n1 < *n2; }
		);

		current = openList.at(0);

		if (*current == endTile)
		{
			std::vector<Node*> path;
			Node* walker = current;
			while (walker->parent != nullptr)
			{
				path.push_back(new Node(*walker));
				walker = walker->parent;
			}
			for (auto& p : pointerBank)
				delete p;
			std::reverse(path.begin(), path.end());
			return path;
		}
		openList.erase(openList.begin()); // Remove first entry
		closedList.push_back(current);		// add the entry to the closed list

		for (int i = 0; i < 9; i++) {
		
			if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8) continue;
			int x = current->tile->getPosition().x;
			int y = current->tile->getPosition().y;
			int xi = (i % 3) - 1;
			int yi = (i / 3) - 1;
			Tile* at = getTile(x + xi,y + yi);
			if (at == nullptr) continue;
			else if (!at->isWalkbale()) continue;

			float gCost = current->gCost + (getDistance(*current->tile, *at) == 1 ? 1 : 0.95);
			float hCost = getDistance(*at, endTile);
			Node* node = new Node(at, current, gCost, hCost);
			pointerBank.push_back(node);
			if (std::find(closedList.begin(), closedList.end(), node) != closedList.end() && gCost >= node->gCost) continue;
			if (std::find(openList.begin(), openList.end(), node) == openList.end() || gCost < node->gCost) openList.push_back(node);
		}

	}
	return std::vector<Node*>();
}

Tile* Grid::getTile(int x, int y) const
{
	if (x < 0 || x > m_tiles[0].size() || y < 0 || y > m_tiles[0].size())
		return nullptr;
	return m_tiles[x][y];
}

