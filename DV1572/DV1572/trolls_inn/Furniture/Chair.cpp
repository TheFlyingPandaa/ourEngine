#include "../../OurEngine.h"
#include "OurEngine.h"
#include "Chair.h"

int Chair::lowPrice = 100;
int Chair::highPrice = 1000;

Chair::Chair(DirectX::XMFLOAT3 pos, MESH meshid, const int setting) : Furniture(pos, meshid)
{
	p_gridSize = 1;
	p_rot = 0;
	p_object.setMesh(MLP::GetInstance().GetMesh(meshid));

	p_object.setPos(pos.x + 0.5f, pos.y - 0.2f, pos.z + 0.5f);
	//Set Whatever suits
	//p_attributes
	if (setting == 0)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
		if (meshid == MESH::CHAIR_LOW)
		{
			p_price = lowPrice;
			p_attributes.SetStat(-0.05f);
		}
		else
		{
			p_price = highPrice;
			p_attributes.SetStat(0.08f);
		}
	}
	else if (setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");

		p_object.GetMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		if (meshid == MESH::CHAIR_LOW)
		{
			p_price = 1000;
		}
		else
		{
			p_price = 2000;
		}
	}
}

Chair::~Chair()
{
}

std::string Chair::WhatType()
{
	return "Chair";
}

Furniture * Chair::MakeCopy()
{
	return new Chair(static_cast<const Chair&>(*this));;
}

int Chair::getPrice(bool low)
{
	if (low)
	{
		return lowPrice;
	}
	else
	{
		return highPrice;
	}
}
