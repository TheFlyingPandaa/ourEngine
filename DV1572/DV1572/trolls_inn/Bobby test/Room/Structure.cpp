#include "Structure.h"

void Tile::setMesh(Mesh * mesh)
{
	this->quad.setMesh(mesh);
}

void Tile::setPosX(const int x)
{
	XMFLOAT3 tempPos = quad.getPosition();
	quad.setPos(static_cast<float>(x), tempPos.y, tempPos.z);
}
void Tile::setPosY(const int y)
{
	XMFLOAT3 tempPos = quad.getPosition();
	quad.setPos(tempPos.x, tempPos.y, static_cast<float>(y));
}

XMFLOAT2 Tile::getPosition() const
{
	XMFLOAT3 posVector = quad.getPosition();
	XMFLOAT2 returnFloat = { posVector.x, posVector.z };
	return returnFloat;
}