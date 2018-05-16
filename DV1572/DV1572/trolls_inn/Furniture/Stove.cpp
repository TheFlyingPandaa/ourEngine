#include "OurEngine.h"
#include "Stove.h"

Stove::Stove(DirectX::XMFLOAT3 pos, MESH meshid, const int setting) : Furniture(pos, meshid)
{
	//TODO:: NOT DONE YET
	p_gridSize = 1;
	p_rot = 90;
	p_object.setPos(pos.x - 20.5f, pos.y - 0.2f, pos.z - 0.5f);
	p_object.setMesh(MLP::GetInstance().GetMesh(meshid));
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

Stove::~Stove()
{
}

std::string Stove::WhatType()
{
	return "Stove";
}

Furniture * Stove::MakeCopy()
{
	return new Stove(static_cast<const Stove&>(*this));
}
