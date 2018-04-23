#pragma once
#include "Furniture.h"

class Table : public Furniture
{
public:
	Table(DirectX::XMFLOAT3 pos, Mesh * mesh);
	~Table();

	//static int getGridSize();
};
