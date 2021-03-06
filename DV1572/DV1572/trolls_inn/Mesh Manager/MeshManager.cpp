#include "MeshManager.h"

Mesh* MeshHandler::wallWithWindow = nullptr; 
Mesh* MeshHandler::door = nullptr;
Mesh* MeshHandler::bed = nullptr; 
Mesh* MeshHandler::chair = nullptr; 
Mesh* MeshHandler::wall = nullptr;
Mesh* MeshHandler::floorTile = nullptr;
Mesh* MeshHandler::grassTile = nullptr;
Mesh* MeshHandler::table = nullptr; 
Mesh* MeshHandler::box = nullptr; 
Mesh* MeshHandler::plainRectangle = nullptr;

Mesh* MeshHandler::getTable()
{
	if (table == nullptr)
	{
		table = new Mesh();
		table->LoadModel("trolls_inn/Resources/Stol.obj");
	}
	return table;
}

Mesh* MeshHandler::getWall()
{
	if (wall == nullptr)
	{
		wall = new Mesh();
		wall->LoadModel("trolls_inn/Resources/wall3.obj");
	}
	return wall;
}

Mesh* MeshHandler::getChair()
{
	if (chair == nullptr)
	{
		chair = new Mesh();
		chair->LoadModel("trolls_inn/Resources/Stol.obj");
	}
	return chair;
}

Mesh* MeshHandler::getGrassTile()
{
	if (grassTile == nullptr)
	{
		grassTile = new Mesh();
		grassTile->LoadModel("trolls_inn/Resources/Grass.jpg");
	}
	return grassTile;
}

Mesh* MeshHandler::getFloorTile()
{
	if (floorTile == nullptr)
	{
		floorTile = new Mesh();
		floorTile->LoadModel("trolls_inn/Resources/floor.obj");
	}
	return floorTile;
}

Mesh* MeshHandler::getWallWithWindow()
{
	if (wallWithWindow == nullptr)
	{
		wallWithWindow = new Mesh();
		wallWithWindow->LoadModel("trolls_inn/Resources/windowForWallTri.obj");
	}
	return wallWithWindow;
}

Mesh* MeshHandler::getBed()
{
	if (bed == nullptr)
	{
		bed = new Mesh();
		bed->LoadModel("trolls_inn/Resources/Bed/LowBed.obj");
	}
	return bed;
}

Mesh * MeshHandler::getDoor()
{
	if (door == nullptr)
	{
		door = new Mesh();
		door->LoadModel("trolls_inn/Resources/door/Door.obj");
	}
	return door;
}

Mesh * MeshHandler::getBox()
{
	if (box == nullptr)
	{
		box = new Mesh();
		box->LoadModel("trolls_inn/Resources/Box.obj");
	}
	return box;
}

Mesh * MeshHandler::getPlainRectangle()
{
	if (plainRectangle == nullptr)
	{
		plainRectangle = new Mesh();
		plainRectangle->MakeRectangle();
		plainRectangle->setDiffuseTexture("trolls_inn/Resources/HUD/plain.png");
	}
	return plainRectangle;
}

void MeshHandler::cleanAll()
{
	if (table != nullptr)
	{
		delete table; 
	}

	if (wall != nullptr)
	{
		delete wall;
	}
	
	if (chair != nullptr)
	{
		delete chair;
	}

	if (grassTile != nullptr)
	{
		delete grassTile;
	}

	if (floorTile != nullptr)
	{
		delete floorTile;
	}

	if (wallWithWindow != nullptr)
	{
		delete wallWithWindow;
	}

	if (bed != nullptr)
	{
		delete bed;
	}

	if (door != nullptr)
	{
		delete door;
	}

	if (box != nullptr)
	{
		delete box;
	}

	if (plainRectangle != nullptr)
		delete plainRectangle;
}
