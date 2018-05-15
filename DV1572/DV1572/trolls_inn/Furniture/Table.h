#pragma once
#include "Furniture.h"

class Table : public Furniture
{
public:
	//The setting is the decider for creepyness and stuff like that
	Table(DirectX::XMFLOAT3 pos, Mesh * mesh, const int setting = 0);
	~Table();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};
