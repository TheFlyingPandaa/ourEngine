#include <iostream>
#include <string>
#include "Room\Room.h"
#include <vector>

const static int HEIGHT = 25;
const static int WIDTH = 25;

#define Try(x) try { \
 x;\
}\
catch (const char* text)\
{\
	std::cout << text;\
}\

Tile* boardtiles = new Tile[HEIGHT * WIDTH];
std::vector<Room*> rooms;

void updateBoardTiles()
{
	for (auto& room : rooms)
	{
		Tile* roomTiles = room->getTiles();

		for (int y = room->getPosition().y; y < room->getHeight() + room->getPosition().y; y++)
		{
			for (int x = room->getPosition().x; x < room->getWidth() + room->getPosition().x; x++)
			{
				int localX = x - room->getPosition().x;
				int localY = y - room->getPosition().y;
				boardtiles[x + y * WIDTH] = roomTiles[localX + localY * room->getWidth()];
			}

		}
	}
}

void printTiles()
{



	// PASTE INTO GLOBAL TILES
	updateBoardTiles();
	// PRINT GLOBAL TILES
	for (int  y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			std::cout << boardtiles[x + y * WIDTH].toString() << " ";
		}
		std::cout << std::endl;
	}
}

void placeModule(int xPos, int yPos, int width, int height)
{
	if (xPos < 0 || yPos < 0 || xPos + width > WIDTH || yPos + height > HEIGHT)
		throw "hejsan";
	
	Room* room = new Room(xPos, yPos, width, height);
	Point* corners = room->getCorners();

	bool topLeftHit = (boardtiles[(corners[0].x - 1) + corners[0].y * WIDTH].type == MODULE ||
						boardtiles[corners[0].x + (corners[0].y - 1)* WIDTH].type == MODULE);

	bool topRightHit = (boardtiles[(corners[1].x + 1) + corners[1].y * WIDTH].type == MODULE ||
						boardtiles[corners[1].x + (corners[1].y - 1)* WIDTH].type == MODULE);
	
	bool bottomRightHit = (boardtiles[(corners[2].x + 1) + corners[2].y * WIDTH].type == MODULE ||
							boardtiles[corners[2].x + (corners[2].y + 1)* WIDTH].type == MODULE);

	bool bottomLeftHit = (boardtiles[(corners[3].x - 1) + corners[3].y * WIDTH].type == MODULE ||
						boardtiles[corners[3].x + (corners[3].y + 1)* WIDTH].type == MODULE);

	std::cout << "TopLeft " << topLeftHit << std::endl;
	std::cout << "TopRight " << topRightHit << std::endl;
	std::cout << "BottomRight " << bottomRightHit << std::endl;
	std::cout << "BottomLeft " << bottomLeftHit << std::endl;

	if (topLeftHit && !topRightHit)
	{
		// Check collision rooms right
		Tile otherRoomTile;

		if (boardtiles[(corners[0].x - 1) + corners[0].y * WIDTH].type == MODULE)
			otherRoomTile = boardtiles[(corners[0].x - 1) + corners[0].y * WIDTH];
		else
			otherRoomTile = boardtiles[corners[0].x + (corners[0].y - 1)* WIDTH];

		Room* otherRoom = otherRoomTile.myRoom;
		Point otherBottomRight = otherRoom->getCorners()[2];
		
		Point PointVector = (corners[0] - room->getPosition()) - (otherBottomRight - otherRoom->getPosition());
		PointVector.abs();
		Point PointVectorHalf = PointVector / 2;
		Point MiddlePoint = (corners[0] - room->getPosition()) + PointVectorHalf;

		room->getTiles()[MiddlePoint.x + MiddlePoint.y * room->getWidth()].type = DOOR;


	}

	rooms.push_back(room);
	updateBoardTiles();
	
}

int main()
{
	
	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		boardtiles[i].type = EMPTY;
	}
	//placeModule(float(WIDTH) * 0.5f - 5.0f, float(HEIGHT) * 0.5f - 5.0f, 10, 10);
	placeModule(5, 5, 4, 4);
	std::cout << "Room 1 (" << rooms[0]->getCorners()[0].x << "," << rooms[0]->getCorners()[0].y << ") ("
		<< rooms[0]->getCorners()[1].x << "," << rooms[0]->getCorners()[1].y << ") ("
		<< rooms[0]->getCorners()[2].x << "," << rooms[0]->getCorners()[2].y << ") ("
		<< rooms[0]->getCorners()[3].x << "," << rooms[0]->getCorners()[3].y << ")\n";
	placeModule(6,9, 4, 4);
	std::cout << "Room 2 (" << rooms[1]->getCorners()[0].x << "," << rooms[1]->getCorners()[0].y << ") ("
		<< rooms[1]->getCorners()[1].x << "," << rooms[1]->getCorners()[1].y << ") ("
		<< rooms[1]->getCorners()[2].x << "," << rooms[1]->getCorners()[2].y << ") ("
		<< rooms[1]->getCorners()[3].x << "," << rooms[1]->getCorners()[3].y << ")\n";
	
	while (true)
	{
		//system("cls");
		printTiles();
		
		std::cout << "X Y Width Height\n";
		int xPos, yPos, width, height;
		std::cin >> xPos >> yPos >> width >> height;
		std::cin.ignore();

		placeModule(xPos, yPos, width, height);
	}
	


	return 0;

}