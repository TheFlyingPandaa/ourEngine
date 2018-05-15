#include "Table.h"
#include "../Mesh Manager/MeshManager.h"

Table::Table(DirectX::XMFLOAT3 pos, MESH mesh, const int size ,const int setting) : Furniture(pos, mesh)
{
	p_gridSize = size;
	p_rot = 90;
	p_object.setMesh(MLP::GetInstance().GetMesh(mesh));
	
	p_object.setPos(pos.x + 0.5f, pos.y - 0.2f, pos.z + 0.5f);
	//Set Whatever suits
	//p_attributes
	if (setting == 0)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
		if (size == 1)
		{
			p_price = 100;
			p_attributes.SetStat(-0.01f);
		}
		else
		{
			p_price = 200;
			p_attributes.SetStat(1.01f);
		}
	}
	else if(setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");
		
		p_object.GetMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		if (size == 1)
		{
			p_price = 1000;
		}
		else
		{
			p_price = 2000;
		}
	}

}

Table::~Table()
{
}

std::string Table::WhatType()
{
	return "Table";
}

Furniture * Table::MakeCopy()
{
	return new Table(static_cast<const Table&>(*this));
}

