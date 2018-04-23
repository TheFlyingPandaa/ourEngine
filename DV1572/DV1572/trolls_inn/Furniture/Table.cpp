#include "Table.h"

Table::Table(DirectX::XMFLOAT3 pos, Mesh * mesh) : Furniture(pos, mesh)
{
	p_gridSize = 1;
	p_rot = 0;

	p_object.setMesh(mesh);
	p_object.setPos(pos.x + 0.5f, pos.y - 0.2f, pos.z + 0.5f);
	//Set Whatever suits
	//p_attributes
}

Table::~Table()
{
}
