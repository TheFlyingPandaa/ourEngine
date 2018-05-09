#include "Bed.h"
#include "../Mesh Manager/MeshManager.h"

Bed::Bed(DirectX::XMFLOAT3 pos, Mesh * mesh, const int setting) : Furniture(pos, mesh)
{
	//TODO:: NOT DONE YET
	p_gridSize = 2;
	p_rot = 90;

	if (!mesh)
	{
		p_object.setMesh(MeshHandler::getBed());
	}
	else {
		p_object.setMesh(mesh);
	}
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

		p_object.getMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
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
void Bed::Draw()
{
	p_object.Draw();
}

Furniture * Bed::MakeCopy()
{
	return new Bed(static_cast<const Bed&>(*this));
}

//13.5,6.5