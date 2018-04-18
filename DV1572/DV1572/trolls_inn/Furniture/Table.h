#pragma once
#include "Furniture.h"

class Table : public Furniture
{
public:
	Table(DirectX::XMINT2 pos, Mesh * mesh);
	~Table();

};
