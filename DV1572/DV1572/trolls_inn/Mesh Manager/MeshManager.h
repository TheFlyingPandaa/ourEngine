#pragma once
#include "../../OurEngine.h"
class MeshHandler
{
public:

	static Mesh* table; 
	static Mesh* wall; 
	static Mesh* chair; 
	static Mesh* grassTile; 
	static Mesh* floorTile; 
	static Mesh* wallWithWindow; 
	static Mesh* bed; 
	static Mesh* door; 
	static Mesh* box;
	static Mesh* plainRectangle;

	static Mesh* getTable();
	static Mesh* getWall(); 
	static Mesh* getChair(); 
	static Mesh* getGrassTile(); 
	static Mesh* getFloorTile(); 
	static Mesh* getWallWithWindow();
	static Mesh* getBed();
	static Mesh* getDoor(); 
	static Mesh* getBox(); 
	static Mesh* getPlainRectangle();

	static void cleanAll(); 
};