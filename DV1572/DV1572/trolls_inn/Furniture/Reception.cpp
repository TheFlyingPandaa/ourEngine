#include "Reception.h"

Reception::Reception(DirectX::XMFLOAT3 pos, MESH meshid, const int setting) : Furniture(pos, meshid)
{
	p_gridSize = 2;
	p_rot = 0;
	p_object.setMesh(MLP::GetInstance().GetMesh(meshid));

	p_object.setPos(pos.x + 0.5f, pos.y - 0.2f, pos.z + 0.5f);
	//Set Whatever suits
	//p_attributes
	if (setting == 0)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
		if (meshid == MESH::RECEPTION_LOW)
		{
			p_price = 100;
		}
		else
		{
			p_price = 1000;
		}
	}
	else if (setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");

		p_object.GetMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		if (meshid == MESH::RECEPTION_LOW)
		{
			p_price = 1000;
		}
		else
		{
			p_price = 2000;
		}
	}
}

Reception::~Reception()
{
}

std::string Reception::WhatType()
{
	return "Reception";
}

Furniture * Reception::MakeCopy()
{
	return new Reception(static_cast<const Reception&>(*this));;
}