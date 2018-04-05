#include "../../ourEngine/interface/Window.h"
#include "Customer.h"
#include "Staff.h"
#include <vector>
#include <DirectXMath.h>
#include <iostream>
using namespace DirectX;

struct Room
{
	XMFLOAT2 pos;
	int roomTiles[8][8];
};
const int roomCount = 5;

int connections[roomCount][roomCount] = { 0 };

void makeRoomConnection(int source, int destination)
{
	connections[source][destination] = 1;
	connections[destination][source] = 1;
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

// Function to print shortest path from source to j
// using parent array
void printPath(int parent[], int j)
{
	// Base Case : If j is source
	if (parent[j] == -1)
		return;

	printPath(parent, parent[j]);

	printf("%d ", j + 1);
}

// A utility function to print the constructed distance
// array
int printSolution(int dist[], int parent[], int src)
{
	printf("Vertex\t  Distance\tPath");
	for (int i = 0; i < roomCount; i++)
	{
		if (i == src) continue;
		printf("\n%d -> %d \t\t %d\t%d ", src + 1, i + 1, dist[i], src + 1);
		printPath(parent, i);
	}
	std::cout << std::endl;
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


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	std::vector<Room> rooms;
	Room room1;
	room1.pos.x = -1;
	room1.pos.y = 1;

	Room room2;
	room2.pos.x = 1;
	room2.pos.y = 1;

	Room room3;
	room3.pos.x = 1;
	room3.pos.y = -1;

	Room room4;
	room4.pos.x = 2;
	room4.pos.y = -1;

	Room room5;
	room5.pos.x = 2;
	room5.pos.y = 1;
	rooms.push_back(room1); rooms.push_back(room2); rooms.push_back(room3); rooms.push_back(room4); rooms.push_back(room5);

	makeRoomConnection(0, 1);
	makeRoomConnection(1, 2);
	makeRoomConnection(2, 3);
	makeRoomConnection(1, 4);

	printConnections();

	dijkstra(connections, 4);
	

	system("pause");



}