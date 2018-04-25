#pragma once
#include "Furniture.h"

class Table : public Furniture
{
public:
	Table(DirectX::XMFLOAT3 pos, Mesh * mesh, const int setting = 0);
	~Table();

	//static int getGridSize();
};
