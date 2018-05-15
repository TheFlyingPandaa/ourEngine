#include "Bed.h"
#include "../Mesh Manager/MeshManager.h"

Bed::Bed(DirectX::XMFLOAT3 pos, MESH meshid, const int setting) : Furniture(pos, meshid)
{
	//TODO:: NOT DONE YET
	p_gridSize = 2;
	p_rot = 90;
	p_object.setPos(pos.x - 20.5f , pos.y - 0.2f, pos.z - 0.5f);
	setPosition(p_object.getPosition());

	if (setting == 0)
	{
		p_price = 100;
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
	}
	else if (setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");

		p_object.GetMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		p_price = 1000;
	}
}
//
Bed::~Bed()
{
}
std::string Bed::WhatType()
{
	return "Bed";
}

Furniture * Bed::MakeCopy()
{
	return new Bed(static_cast<const Bed&>(*this));
}

//13.5,6.5