#include "../../ourEngine/interface/Window.h"
#include "Customer.h"
#include "CustomerFlowControl.h"
#include "Staff.h"
#include <vector>
#include <DirectXMath.h>
#include <iostream>
using namespace DirectX;

const int roomCount = 5;

int connections[roomCount][roomCount] = { 0 };

struct Node
{
	XMFLOAT2 tile;
	Node* parent;
	float fCost, gCost, hCost;
	Node(XMFLOAT2 tile, Node* parent, float gCost, float hCost)
		:tile(tile), parent(parent), gCost(gCost), hCost(hCost) {}
	bool operator>(const Node& other) const
	{
		return fCost > other.fCost;
	}
	bool operator==(const XMFLOAT2& other) const
	{
		return tile.x == other.x && tile.y == other.y;
	}
};


enum RoomType
{
	HallRoom,
	BedRoom,
	DrinkRoom,
	EatRoom,
	EntranceRoom
};

struct Room
{
	XMFLOAT2 pos;
	RoomType type;
	std::vector<int> roomPaths[roomCount];
	int roomTiles[8][8];
};
std::vector<Room> rooms;

void makeRoomConnection(int source, int destination)
{
	connections[source][destination] = 1;
	connections[destination][source] = 1;
}
void printRoomPath(int src, int dst)
{
	std::cout << src + 1 << "->" << dst + 1 << " ";
	for (int i = 0; i < rooms.at(src).roomPaths[dst].size(); i++)
	{
		std::cout << rooms.at(src).roomPaths[dst].at(i) << " ";
	}
}
void printConnections()
{
	std::cout << " ";
	for (int i = 0; i < roomCount; i++)
		std::cout <<" "<< i + 1;
	std::cout << std::endl;
	for (int i = 0; i < roomCount; i++)
	{
		std::cout << i + 1 << " ";
		for (int j = 0; j < roomCount; j++)
		{
			std::cout << connections[i][j] << " ";
		}
		std::cout << std::endl;


	}
}
int minDistance(int dist[], bool sptSet[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < roomCount; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}


void travelPath(int parent[], int j, int src, int dst)
{
	if (parent[j] == -1)
		return;

	travelPath(parent, parent[j], src, dst);
	// Put path in room
	rooms.at(src).roomPaths[dst].push_back(j + 1);
}

int printSolution(int dist[], int parent[], int src)
{

	for (int i = 0; i < roomCount; i++)
	{
		if (i != src)
			travelPath(parent, i, src, i);
	}
	return src;
}

void dijkstra(int graph[roomCount][roomCount], int src)
{
	int dist[roomCount];  // The output array. dist[i] will hold
				  // the shortest distance from src to i

				  // sptSet[i] will true if vertex i is included / in shortest
				  // path tree or shortest distance from src to i is finalized
	bool sptSet[roomCount];

	// Parent array to store shortest path tree
	int parent[roomCount];

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < roomCount; i++)
	{
		parent[src] = -1;
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}

	// Distance of source vertex from itself is always 0
	dist[src] = 0;
	bool check[roomCount] = { false };
	// Find shortest path for all vertices
	for (int count = 0; count < roomCount - 1; count++)
	{
		// Pick the minimum distance vertex from the set of
		// vertices not yet processed. u is always equal to src
		// in first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the
		// picked vertex.
		for (int v = 0; v < roomCount; v++)

			// Update dist[v] only if is not in sptSet, there is
			// an edge from u to v, and total weight of path from
			// src to v through u is smaller than current value of
			// dist[v]
			if (!sptSet[v] && graph[u][v] &&
				dist[u] + graph[u][v] < dist[v])
			{
				parent[v] = u;
				check[v] = true;
				dist[v] = dist[u] + graph[u][v];
			}

	}

	// print the constructed distance array
	printSolution(dist, parent, src);
}

float getDistance(XMFLOAT2 tile, XMFLOAT2 goal)
{
	XMVECTOR xmTile = XMLoadFloat2(&tile);
	XMVECTOR xmGoal = XMLoadFloat2(&goal);
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}



int getEntrance()
{
	int index = -1;
	for (int i = 0; i < roomCount && index == -1; i++)
	{
		if (rooms.at(i).type == EntranceRoom)
			index = i;
	}
	return index;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	
	std::vector<Customer> customers;
	
	// Create rooms
	Room room1;
	room1.pos.x = -1;
	room1.pos.y = 1;
	room1.type = BedRoom;

	Room room2;
	room2.pos.x = 1;
	room2.pos.y = 1;
	room2.type = HallRoom;

	Room room3;
	room3.pos.x = 1;
	room3.pos.y = -1;
	room3.type = EntranceRoom;

	Room room4;
	room4.pos.x = 2;
	room4.pos.y = -1;
	room4.type = DrinkRoom;

	Room room5;
	room5.pos.x = 2;
	room5.pos.y = 1;
	room5.type = EatRoom;
	rooms.push_back(room1); rooms.push_back(room2); rooms.push_back(room3); rooms.push_back(room4); rooms.push_back(room5);
	
	// Create Connections which go both ways
	makeRoomConnection(0, 1);
	makeRoomConnection(1, 2);
	makeRoomConnection(2, 3);
	makeRoomConnection(1, 4);

	printConnections();

	// Create route network for everyroom.
	for(int i = 0; i < roomCount; i++)
		dijkstra(connections, i);
	
	std::cout << std::endl;
	
	CustomerFlowControl cFL;

	customers.push_back(cFL.update());

	std::cout << customers.back().GetRaceStr() << " is " << customers.back().GetActionStr() << std::endl;

	int entanceIndex = getEntrance();

	printRoomPath(0, 3);
	
	
	

	system("pause");



}