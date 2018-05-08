#include "Table.h"

Table::Table(DirectX::XMFLOAT3 pos, Mesh * mesh, const int setting) : Furniture(pos, mesh)
{
	p_gridSize = 1;
	p_rot = 0;

	p_object.setMesh(mesh);
	p_object.setPos(pos.x + 0.5f, pos.y - 0.2f, pos.z + 0.5f);
	//Set Whatever suits
	//p_attributes
	if (setting == 0)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableNormalStats.txt");
		p_price = 100;
	}
	else if(setting == 1)
	{
		//LoadFurnitureStats("trolls_inn/Furniture/FurnitureStats/TableCreepyStats.txt");
		
		p_object.getMesh()->setDiffuseTexture("trolls_inn/Resources/StolTextureEvil.bmp");
		p_price = 1000;
	}

}

Table::~Table()
{
}
