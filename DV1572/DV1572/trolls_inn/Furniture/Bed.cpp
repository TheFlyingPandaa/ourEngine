#include "OurEngine.h"
#include "Bed.h"

int Bed::lowPrice = 100;
int Bed::highPrice = 1000;
Bed::Bed(DirectX::XMFLOAT3 pos, MESH meshid, const int setting) : Furniture(pos, meshid)
{
	//TODO:: NOT DONE YET
	p_gridSize = 2;
	p_rot = 90;
	p_object.setPos(pos.x - 20.5f , pos.y - 0.2f, pos.z - 0.5f);
	p_object.setMesh(MLP::GetInstance().GetMesh(meshid));
	setPosition(p_object.getPosition());

	if (setting == 0)
	{
		if (meshid == MESH::BED_LOW)
		{
			p_price = lowPrice;
			p_attributes.SetStat(-0.08f);
		}
		else
		{
			p_price = highPrice;
			p_attributes.SetStat(0.08f);
		}
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
	}
	else if (setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");

		p_object.GetMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		if (meshid == MESH::BED_LOW)
		{
			p_price = 1000;
		}
		else
		{
			p_price = 2000;
		}
		
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

int Bed::getPrice(bool low)
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

//13.5,6.5