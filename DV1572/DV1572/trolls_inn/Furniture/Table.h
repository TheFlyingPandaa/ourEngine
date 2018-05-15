#pragma once
#include "Furniture.h"

class Table : public Furniture
{
public:
	//The setting is the decider for creepyness and stuff like that
	Table(DirectX::XMFLOAT3 pos, MESH mesh, const int size = 1, const int setting = 0);
	~Table();

	std::string WhatType() override;
	Furniture * MakeCopy() override;
	//static int getGridSize();
};
